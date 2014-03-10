/* ppaml/tracer.h -- PPAML timing instrumentation interface
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
 * @file tracer.h
 * Library interface.
 *
 * This file defines the top-level interface for ppamltracer.
 *
 * Since all the functions in this file operate on @ref ppaml_tracer_t and @ref
 * ppaml_phase_t structs, their documentation has been moved to the relevant
 * data structure pages.
 */

#ifndef PPAML_TRACER_H
#define PPAML_TRACER_H

#include "tracer/internal.h"	// for ppaml_tracer_t and ppaml_phase_t

/**
 * @name Resource management
 *
 * @{
 */

/**
 * Initializes a ppaml_tracer_t.
 *
 * @relates ppaml_tracer_t
 *
 * The trace report will be stored in Open Trace Format; all trace file paths
 * will begin with @p report_name_base.
 *
 * @param[out] tracer pointer to the tracer to be initialized
 * @param[in] report_name_base base file name for the report files
 *
 * @pre @p tracer is nonnull.
 * @pre @p *tracer is uninitialized.
 *
 * @return 0 upon success
 * @return 1 if the Open Trace Format file manager could not be initialized
 * @return 2 if the Open Trace Format writer could not be initialized
 * @return 3 if setting the trace resolution failed
 * @return 4 if defining the main OTF process failed
 *
 * @post If the function return successfully, @p *tracer is initialized.
 */
int ppaml_tracer_init(ppaml_tracer_t *tracer, const char report_name_base[]);

/**
 * Initializes a ppaml_tracer_t, taking settings from environment variables.
 *
 * @relates ppaml_tracer_t
 *
 * The trace report will be stored in Open Trace Format; all trace file paths
 * will begin with the value in the environment variable @c
 * PPAMLTRACER_TRACE_BASE.
 *
 * @param[out] tracer pointer to the tracer to be initialized
 *
 * @pre @p tracer is nonnull.
 * @pre @p *tracer is uninitialized.
 *
 * @return 0 upon success
 * @return 1 if the Open Trace Format file manager could not be initialized
 * @return 2 if the Open Trace Format writer could not be initialized
 * @return 3 if setting the trace resolution failed
 * @return 4 if defining the main OTF process failed
 * @return 5 if @c PPAMLTRACER_TRACE_BASE is unset or empty
 *
 * @post If the function return successfully, @p *tracer is initialized.
 */
int ppaml_tracer_init_from_env(ppaml_tracer_t *tracer);

/**
 * Finalizes a ppaml_tracer_t.
 *
 * @relates ppaml_tracer_t
 *
 * @param[out] tracer pointer to the tracer to be finalized
 *
 * @pre @p tracer is nonnull.
 * @pre @p *tracer is initialized.
 *
 * @return 0 upon success
 * @return 1 if the Open Trace Format writer could not be closed
 *
 * @post If the function return successfully, @p *tracer is uninitialized.
 */
int ppaml_tracer_done(ppaml_tracer_t *tracer);

/**
 * Initializes a ppaml_phase_t.
 *
 * @relates ppaml_phase_t
 *
 * @param[in] tracer pointer to the tracer which records this phase
 * @param[out] phase pointer to the phase to be initialized
 * @param[in] name name of the phase
 *
 * @pre @p tracer is nonnull.
 * @pre @p *tracer is initialized.
 * @pre @p phase is nonnull.
 * @pre @p *phase is uninitialized.
 *
 * @return 0 upon success
 * @return 1 if defining the phase failed
 *
 * @post If the function return successfully, @p *phase is initialized.
 */
int ppaml_phase_init(
	ppaml_tracer_t *tracer,
	ppaml_phase_t *phase,
	const char name[]);

/**
 * Finalizes a ppaml_phase_t.
 *
 * @relates ppaml_phase_t
 *
 * @param[out] phase pointer to the phase to be finalized
 *
 * @pre @p phase is nonnull.
 * @pre @p *phase is initialized.
 *
 * @return 0 upon success; a nonzero return indicates failure.
 *
 * @post If the function return successfully, @p *phase is uninitialized.
 */
int ppaml_phase_done(ppaml_phase_t *phase);

/// @}

/**
 * @name Timing
 *
 * @{
 */

/**
 * Records the start of a phase.
 *
 * @relates ppaml_phase_t
 *
 * @param[in] phase pointer to the phase
 *
 * @pre @p phase is nonnull.
 * @pre @p *phase is initialized.
 *
 * @return 0 upon success
 * @return 1 if getting the time failed
 * @return 2 if recording the phase start failed
 */
int ppaml_phase_start(ppaml_phase_t *phase);

/**
 * Records the end of a phase.
 *
 * @relates ppaml_phase_t
 *
 * @param[in] phase pointer to the phase
 *
 * @pre @p phase is nonnull.
 * @pre @p *phase is initialized.
 *
 * @return 0 upon success
 * @return 1 if getting the time failed
 * @return 2 if recording the phase stop failed
 */
int ppaml_phase_stop(ppaml_phase_t *phase);

/// @}

#endif
