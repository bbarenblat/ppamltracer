#lang scribble/manual
@;{ manual.scrbl -- Scribble documentation for ppamltracer-racket
Copyright (C) 2013, 2014  Galois, Inc.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. Neither Galois's name nor the names of other contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }

@(require (for-label racket/base
                     racket/sequence))
@(require (for-label ppamltracer))

@title{ppamltracer}

ppamltracer is a lightweight, portable tracing library designed for explicit instrumention of generated code.
If you're writing a compiler and need hard data on your optimizer's efficacy, ppamltracer is the library for you.
This Racket library provides a high-level API on top of the libppamltracer C API.

ppamltracer's usage can be summed up in a couple lines:

@racketblock[
(require ppamltracer)
(let/tracer [tracer "/tmp/my_report"]
  (let/phase tracer [phase "phase 1"]
    (with-phase-running phase
      (do-stuff)))
  (let/phase tracer [phase "phase 2"]
    (with-phase-running phase
      (do-other-stuff))
    (with-phase-running phase
      (do-yet-more-stuff))))
]

This creates a report with the total runtime of @racket[do-stuff] recorded as "phase 1" and the total runtime of @racket[do-other-stuff] and @racket[do-yet-more-stuff] combined as "phase 2".

@secref{getting-started} contains a more lengthy example.

ppamltracer writes trace logs in the @hyperlink["http://tu-dresden.de/zih/otf/"]{Open Trace Format}, a free and open standard developed by the Zentrum für Informationsdienste und Hochleistungsrechnen (Center for Information Services and High-Performance Computing) at the Technical University of Dresden.

We developed ppamltracer as part of DARPA's @hyperlink["http://darpa.mil/Our_Work/I2O/Programs/Probabilistic_Programming_for_Advanced_Machine_Learning_(PPAML).aspx"]{Probabilistic Programming for Advancing Machine Learning} (PPAML) project.


@section[#:tag "getting-started"]{Getting Started}

This basic example demonstrates using ppamltracer to trace two functions -- a factorial function and a Fibonacci number generator.

@#reader scribble/comment-reader (racketblock
;; simple.rkt -- basic ppamltracer example
;; This file is in the public domain.

(require racket/sequence)
(require ppamltracer)

(define (fact phase n)
  ;; Record that we're running inside the factorial phase.
  (with-phase-running phase
    ;; Compute the factorial.
    (if (= n 0)
        1
        (* n (fact phase (- n 1))))))

(define (fib phase n)
  ;; Record that we're running inside the Fibonacci phase.
  (with-phase-running phase
    ;; Compute the nth Fibonacci number.
    (case n
      [(0 1) n]
      [else (+ (fib phase (- n 1)) (fib phase (- n 2)))])))

;; Start ppamltracer.
(let/tracer [tracer "/tmp/simple_report"]
  ;; Register the factorial phase.
  (let/phase tracer [phase "fact"]
    ;; Print factorials.
    (printf "Factorials: ")
    (sequence-for-each (lambda (n) (printf "~a " (fact phase n)))
                       (in-range 40))
    (printf "\n"))
  ;; Register the Fibonacci phase.
  (let/phase tracer [phase "fib"]
    ;; Print Fibonacci numbers.
    (printf "Fibonacci numbers: ")
    (sequence-for-each (lambda (n) (printf "~a " (fib phase n)))
                       (in-range 16))
    (printf "\n")))
)

@section{API in Depth}

@defmodule[ppamltracer]

@subsection{Tracers}

ppamltracer is fundamentally a set of stateful operations; the @racket[tracer] data type holds the state ppamltracer needs to operate properly.

@defproc[(tracer? [obj any/c])
         boolean?]{
  Evaluates to @racket[#t] if and only if @racket[obj] is a @racket[tracer].
}

@defproc*[([(call-with-tracer [proc (-> tracer? A)])
            A]
           [(call-with-tracer [report-name-base path-string?]
                              [proc (-> tracer? A)])
            A])]{
  Creates a new @racket[tracer].
  The @racket[tracer] will save trace reports in Open Trace Format.
  Should @racket[report-name-base] be specified, all trace file paths will begin with @racket[report-name-base]; otherwise, all trace file paths will begin with the contents of the environment variable @envvar{PPAMLTRACER_TRACE_BASE}.

  Invoking @racket[call-with-tracer] without specifying @racket[report-name-base] is an error if @envvar{PPAMLTRACER_TRACE_BASE} is undefined.
  Doing so will cause Racket to throw (a sub-exception of) @racket[exn:fail:ppamltracer:configuration].
}

@defform*[#:id let/tracer
          #:literals (tracer)
          [(let/tracer [tracer] body ...)
           (let/tracer [tracer report-name-base] body ...)]
          #:contracts ([report-name-base path-string?])]{
  Macro version of @racket[call-with-tracer].
  Equivalent to one of

  @racketblock[
  (call-with-tracer (lambda (tracer) body ...))
  (call-with-tracer report-name-base (lambda (tracer) body ...))
  ]
}

@subsection{Phases}

A @racket[phase] represents a phase of execution about which the user wishes to gather timing statistics.

@defproc[(phase? [obj any/c])
         boolean?]{
  Evaluates to @racket[#t] if and only if @racket[obj] is a @racket[phase].
}

@defproc[(call-with-phase [tracer tracer?]
                          [phase-name string?]
                          [proc (-> phase? A)])
         A]{
  Creates a new @racket[phase] attached to @racket[tracer] and named @racket[phase-name].
}

@defform[#:id let/phase
         #:literals (phase)
         (let/phase tracer [phase name] body ...)
         #:contracts ([tracer tracer?]
                      [name string?])]{
  Macro version of @racket[call-with-phase].
  Equivalent to

  @racketblock[
  (call-with-phase tracer name (lambda (phase) body ...))
  ]
}

@subsection{Tracing}

@defproc[(call-with-phase-running [phase phase?]
                                  [proc (-> A)])
         A]{
  Runs @racket[proc], associating information about its timing and execution with @racket[phase] in the trace report.
  Should @racket[proc] throw an exception, @racket[call-with-phase-running] will stop tracing before propagating the exception up the stack.
}

@defform[#:id with-phase-running
         #:literals (phase)
         (with-phase-running phase body ...)
         #:contracts ([phase phase?])]{
  Macro version of @racket[call-with-phase-running].
  Equivalent to

  @racketblock[
  (call-with-phase-running phase (lambda () body ...))
  ]
}

