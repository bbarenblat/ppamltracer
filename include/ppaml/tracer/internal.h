/* ppaml/tracer/internal.h -- definitions of abstract types
 * Copyright (C) 2013  Galois, Inc.
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at
 * <http://corp.galois.com/contact/> or write to Galois, Inc., 421 Southwest
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

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
