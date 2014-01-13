#lang racket/base
#| ppamltracer -- Racket bindings to ppamltracer
Copyright (C) 2013, 2014  Galois, Inc.

This library is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this library.  If not, see <http://www.gnu.org/licenses/>.

To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
Oregon, 97204-1622. |#

(require (for-syntax racket/base
                     racket/syntax
                     (only-in racket/list drop-right)
                     (only-in racket/string string-join)
                     (only-in srfi/13 string-tokenize)
                     (only-in srfi/14 char-set char-set-complement)
                     srfi/26; hole notation
                     ))

(require (rename-in ffi/unsafe [-> ~>])
         ffi/unsafe/define)
(require racket/contract)

#| A shorthand for 'let-syntax-rule' with only one macro.  For example,

    (let-syntax-rule (take-fst x y) x
      (take-fst 'foo 'bar))

expands to

    'foo
|#
(define-syntax-rule (let-syntax-rule (name . args) template body)
  (let-syntax ([name (syntax-rules () [(_ . args) template])])
    body))

;; Convenience macro to instantiate an exception at the current location.
(define-syntax-rule (fail exception args ...)
  (apply exception (list args ... (current-continuation-marks))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Low-level interface ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Load the library.
(define-ffi-definer define-ppamltracer (ffi-lib "libppamltracer"))

#| We're going to model ppaml_tracer_t and ppaml_phase_t as abstract types by
having Racket treat them as buffers.  These two symbols define how long those
buffers need to be.  Ideally, they should be of type 'size_t', but Racket
doesn't have a size_t builtin, so we just use the largest C unsigned integer
type. |#
(define-ppamltracer ppaml_tracer_t_size _uint64)
(define-ppamltracer ppaml_phase_t_size _uint64)

#| Define the struct types we're working with as buffer types.  Instead of
mucking around with initializers and finalizers, which we can't seem to get
working correctly, we simply export an API which enforces proper resource
cleanup. |#
(define (make-buffer size) (make-ctype (_bytes o size) #f #f))
(define _ppaml_tracer_t (make-buffer ppaml_tracer_t_size))
(define _ppaml_phase_t (make-buffer ppaml_phase_t_size))

#| To help avoid passing structs of the wrong type in API internals, we'll
manipulate the structs as tagged pointers. |#
(define _tracer (_cpointer 'ppaml_tracer_t))
(define _phase (_cpointer 'ppaml_phase_t))

;; Allocation functions for _tracer and _phase.
(define (malloc-tagged-pointer size-or-type tag)
  (let ([result (malloc size-or-type 'atomic)])
    (cpointer-push-tag! result tag)
    result))
(define (malloc-_tracer)
  (malloc-tagged-pointer _ppaml_tracer_t 'ppaml_tracer_t))
(define (malloc-_phase)
  (malloc-tagged-pointer _ppaml_phase_t 'ppaml_phase_t))

;; Import library functions.
(define-ppamltracer ppaml_tracer_init (_fun _tracer _path ~> _int))
(define-ppamltracer ppaml_tracer_done (_fun _tracer ~> _int))
(define-ppamltracer ppaml_phase_init (_fun _tracer _phase _string ~> _int))
(define-ppamltracer ppaml_phase_done (_fun _phase ~> _int))
(define-ppamltracer ppaml_phase_start (_fun _phase ~> _int))
(define-ppamltracer ppaml_phase_stop (_fun _phase ~> _int))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Error handling ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#| Convenience macro to create an exception.  For example,

    (define-exception exn:fail:whatever)

expands to

    (struct exn:fail:whatever exn:fail ()
            #:extra-constructor-name make-exn:fail:whatever
            #:transparent)

You can also call 'define-exception' with a message.  Then, instead of creating
an extra binary make-prefixed constructor, 'define-exception' will create an
extra /unary/ make-prefixed constructor.  All instances of the exception will
then have the same message.  For example,

    (define-exception exn:fail:whatever "whatever happened!")

expands to

    (struct exn:fail:whatever exn:fail ()
            #:transparent)
    (define (make-exn:fail:whatever continuation-mark-set)
      (exn:fail:whatever "whatever happened!" continuation-mark-set))
|#
(define-syntax (define-exception stx)
  (let ([#| Lifts a function which operates on data to operate on syntax
         instead. |#
         map-as-datum
         (lambda (f stx) (datum->syntax stx (f (syntax->datum stx))))]
        [#| Given an exception name (as a symbol), produces the conventional
         super-exception.  For instance,

             (take-base-exn 'exn:fail:whatever)

         expands to

             'exn:fail
         |#
         take-base-exn
         (compose1 string->symbol
                   (cut string-join <> ":")
                   (cut drop-right <> 1)
                   (cut string-tokenize <> (char-set-complement
                                            (char-set #\:)))
                   symbol->string)])
    (syntax-case stx ()
      [(_ name)
       (with-syntax ([base-name (map-as-datum take-base-exn #'name)]
                     [make-name (format-id stx "make-~a" #'name
                                           #:source #'name)])
         #'(struct name base-name ()
                   #:extra-constructor-name make-name
                   #:transparent))]
      [(_ name message)
       (with-syntax ([base-name (map-as-datum take-base-exn #'name)]
                     [make-name (format-id stx "make-~a" #'name
                                           #:source #'name)])
         #'(begin
             (struct name base-name () #:transparent)
             (define (make-name continuation-mark-set)
               (name message continuation-mark-set))))])))

#| Like define-exception, but does not create an extra make-prefixed
constructor at all. |#
(define-syntax (define-exception* stx)
  (let ([map-as-datum
         (lambda (f stx) (datum->syntax stx (f (syntax->datum stx))))]
        [take-base-exn
         (compose1 string->symbol
                   (cut string-join <> ":")
                   (cut drop-right <> 1)
                   (cut string-tokenize <> (char-set-complement
                                            (char-set #\:)))
                   symbol->string)])
    (syntax-case stx ()
      [(_ name)
       (with-syntax ([base-name (map-as-datum take-base-exn #'name)])
         #'(struct name base-name () #:transparent))])))

;;;;; Exception hierarchy ;;;;;

(define-exception exn:fail:ppamltracer)

(define-exception exn:fail:ppamltracer:otf)

(define-exception exn:fail:ppamltracer:otf:manager)

(define-exception exn:fail:ppamltracer:otf:manager:initialization
  "could not initialize Open Trace Format file manager")

(define-exception exn:fail:ppamltracer:otf:writer)

(define-exception exn:fail:ppamltracer:otf:writer:initialization
  "could not open Open Trace Format writer")

(define-exception exn:fail:ppamltracer:otf:writer:phase-definition
  "could not define phase")

(define-exception exn:fail:ppamltracer:otf:writer:entry
  "could not record phase start")

(define-exception exn:fail:ppamltracer:otf:writer:exit
  "could not record phase end")

(define-exception exn:fail:ppamltracer:otf:writer:close
  "could not close Open Trace Format writer")

(define-exception exn:fail:ppamltracer:otf:writer:resolution
  "could not set trace resolution")

(define-exception* exn:fail:ppamltracer:otf:writer:process-definition)
(define (make-exn:fail:ppamltracer:otf:writer:process-definition
         process-name
         continuation-mark-set)
  (exn:fail:ppamltracer:otf:writer:process-definition
   (format "could not define Open Trace Format process ~v" process-name)
   continuation-mark-set))

(define-exception exn:fail:ppamltracer:timing)

(define-exception exn:fail:ppamltracer:timing:clock-acquisition
  "could not get current time")


;;;;; Error-checked versions of library functions ;;;;;

#| This macro makes checking C return codes trivial.  Invocation is analogous
to that for the 'cond' form:

    (define some-function
      (checked some-c-function
               [0 'ok]
               [1 (exn make-exn:fail:ppamltracer "bad data format")]
               [-1 'ok]))

This defines 'some-function' as a function taking an arbitrary number of
arguments.  'some-function', when invoked, will pass all its arguments to
'some-c-function'.  If 'some-c-function' returns 0 or -1, then some-function
will evaluate successfully to whatever 'some-c-function' returned; if
'some-c-function' returns 1, though, 'some-function' will raise an
'exn:fail:ppamltracer' exception with the specified error message.

If 'some-c-function' returns an unexpected value, 'some-function' will raise an
'exn:fail' noting the actual return value and declaring it a bug. |#
(define-syntax-rule (checked proc [expected consequence] ...)
  (let-syntax-rule (cond-rhs result consequence-or-exception)
      (if (eq? consequence-or-exception 'ok)
          result
          (raise consequence-or-exception))
    (lambda args
      (let ([result (apply proc args)])
        (case result
            [(expected) (cond-rhs result consequence)]
            ...
            [else (error (quote proc)
                         "unexpected return value ~v\n\
This is a bug in ppamltracer!  Report it to the maintainers."
                         result)])))))

(define ((as-void proc) . args)
  (void (apply proc args)))

(define ppaml-tracer-init
  (as-void
   (checked ppaml_tracer_init
            [0 'ok]
            [1 (fail make-exn:fail:ppamltracer:otf:manager:initialization)]
            [2 (fail make-exn:fail:ppamltracer:otf:writer:initialization)]
            [3 (fail make-exn:fail:ppamltracer:otf:writer:resolution)]
            [4 (fail make-exn:fail:ppamltracer:otf:writer:process-definition
                     "main")])))

(define ppaml-tracer-done
  (as-void
   (checked ppaml_tracer_done
            [0 'ok]
            [1 (fail make-exn:fail:ppamltracer:otf:writer:close)])))

(define ppaml-phase-init

  (as-void
   (checked ppaml_phase_init
            [0 'ok]
            [1 (fail make-exn:fail:ppamltracer:otf:writer:phase-definition)])))

(define ppaml-phase-start
  (as-void
   (checked ppaml_phase_start
            [0 'ok]
            [1 (fail make-exn:fail:ppamltracer:timing:clock-acquisition)]
            [2 (fail make-exn:fail:ppamltracer:otf:writer:entry)])))

(define ppaml-phase-stop
  (as-void
   (checked ppaml_phase_stop
            [0 'ok]
            [1 (fail make-exn:fail:ppamltracer:timing:clock-acquisition)]
            [2 (fail make-exn:fail:ppamltracer:otf:writer:exit)])))

(define ppaml-phase-done
  (as-void
   (checked ppaml_phase_done
            [0 'ok])))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; High-level API ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#| The functions in this section comprise a relatively safe API for this
library--i.e., using them will help avoid resource leaks.  Of course, Racket
gives you plenty of rope to hang yourself--you could, for instance, call/cc
your way back into the middle of some traced function. |#

;; Predicates on '_tracer' and '_phase', to emulate declaration as structs.
(define (cpointer-with-tag? obj tag)
  (and (cpointer? obj)
       (cpointer-has-tag? obj tag)))
(define (tracer? obj) (cpointer-with-tag? obj 'ppaml_tracer_t))
(define (phase? obj) (cpointer-with-tag? obj 'ppaml_phase_t))

#| Execute a procedure in an exception-safe environment such that its resources
are properly released upon exit.  With the exception handling stripped away,

    (managed-execution resource initialize initialize-args proc finalize)

evaluates to

    (begin (apply initialize (cons resource initialize-args))
           (proc resource)
           (finalize resource))
|#
(define (managed-execution resource initialize initialize-args proc finalize)
  (apply initialize (cons resource initialize-args))
  (let ([result
         (with-handlers
             ([exn? (lambda (exn)
                      #| An exception occurred during the execution of 'proc'.
                      Clean up and re-throw. |#
                      (with-handlers
                          ([exn? (lambda (exn)
                                   #| An exception occurred during cleanup!
                                   We're already responding to an existing
                                   exception, though, so just ignore it. |#
                                   (void))])
                        (finalize resource))
                      (raise exn))])
           (proc resource))])
    (finalize resource)
    result))

(define (call-with-tracer report-name-base proc)
  (managed-execution (malloc-_tracer)
                     ppaml-tracer-init
                     (list report-name-base)
                     proc
                     ppaml-tracer-done))

(define (call-with-phase tracer phase-name proc)
  (managed-execution (malloc-_phase)
                     (lambda (phase tracer name)
                       ;; Reorder arguments to fit C API.
                       (ppaml-phase-init tracer phase name))
                     (list tracer phase-name)
                     proc
                     ppaml-phase-done))

(define (call-with-phase-running phase proc)
  (managed-execution phase
                     ppaml-phase-start
                     '()
                     (lambda (ignored) (proc))
                     ppaml-phase-stop))


;;;;; Macro versions ;;;;;

(define-syntax-rule (let/tracer [tracer report-name-base] body ...)
  (call-with-tracer report-name-base (lambda (tracer) body ...)))

(define-syntax-rule (let/phase tracer [phase phase-name] body ...)
  (call-with-phase tracer phase-name (lambda (phase) body ...)))

(define-syntax-rule (with-phase-running phase body ...)
  (call-with-phase-running phase (lambda () body ...)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Module interface ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(provide
 (contract-out [tracer? (-> any/c boolean?)]
               [phase? (-> any/c boolean?)]
               [call-with-tracer (parametric->/c [A]
                                   (-> path-string? (-> tracer? A) A))]
               [call-with-phase (parametric->/c [A]
                                  (-> tracer? string? (-> phase? A) A))]
               [call-with-phase-running (parametric->/c [A]
                                          (-> phase? (-> A) A))])
 let/tracer
 let/phase
 with-phase-running)