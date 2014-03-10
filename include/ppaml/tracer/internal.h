/* ppaml/tracer/internal.h -- definitions of abstract types
 * Copyright (C) 2013, 2014  Galois, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither Galois's name nor the names of other contributors may be used to
 *      endorse or promote products derived from this software without specific
 *      prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

/**
 * @file internal.h
 * Definitions of abstract types.
 *
 * @warning Do not include this file directly.  Instead, include @ref
 * ppaml/tracer.h.
 *
 * This file contains the implementations of abstract types which, for
 * efficiency reasons, are exposed to the user.
 */

/* THIS FILE SHOULD NOT BE #INCLUDED DIRECTLY.  Instead, #include
 * <ppaml/tracer.h>. */

#ifndef PPAML_TRACER_INTERNAL_H
#define PPAML_TRACER_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#include <otf.h>

/**
 * @struct ppaml_tracer_t ppaml/tracer/internal.h ppaml/tracer.h
 * Tracer state bundle.
 *
 * ppamltracer is fundamentally a set of stateful operations; this struct holds
 * the state ppamltracer needs to operate properly.
 *
 * To increase flexibility, ppamltracer users may allocate instances of this
 * struct on the stack or heap.
 *
 * Newly-allocated instances of ppaml_tracer_t are invalid and must be
 * initialized using @ref ppaml_tracer_init before they can be used.  Once a
 * ppaml_tracer_t is no longer required, it must be finalized using @ref
 * ppaml_tracer_done.
 */
typedef struct {
	/// @private The underlying Open Trace Format file manager.
        OTF_FileManager *manager;
	/// @private The underlying Open Trace Format output subsystem.
        OTF_Writer *writer;
} ppaml_tracer_t;

/**
 * `sizeof(`@ref ppaml_tracer_t`)`.
 *
 * @warning This constant is intended for interfacing with other languages.  It
 * is not part of the stable ppamltracer C API.  Use
 * `sizeof(`@ref ppaml_tracer_t`)` instead.
 */
const size_t ppaml_tracer_t_size;

/**
 * @struct ppaml_phase_t ppaml/tracer/internal.h ppaml/tracer.h
 * A phase of execution to trace.
 *
 * A ppaml_phase_t represents a phase of execution about which the user wishes
 * to gather timing statistics.
 *
 * To increase flexibility, ppamltracer users may allocate instances of this
 * struct on the stack or heap.
 *
 * Newly-allocated instances of ppaml_phase_t are invalid and must be
 * initialized using @ref ppaml_phase_init before they can be used.  Once a
 * ppaml_phase_t is no longer required, it must be finalized using @ref
 * ppaml_phase_done.
 */
typedef struct {
	/// @private The associated ppaml_tracer_t.
        ppaml_tracer_t *tracer;
	/**
	 * @private
	 * The Open Trace Format identifier for this phase.
	 *
	 * Phases are isomorphic to functions as understood by the Open Trace
	 * Format libraries; thus, this is a "function identifier".
	 */
	uint32_t otf_function_id;
} ppaml_phase_t;

/**
 * `sizeof(`@ref ppaml_phase_t`)`.
 *
 * @warning This constant is intended for interfacing with other languages.  It
 * is not part of the stable ppamltracer C API.  Use
 * `sizeof(`@ref ppaml_phase_t`)` instead.
 */
const size_t ppaml_phase_t_size;

#endif
