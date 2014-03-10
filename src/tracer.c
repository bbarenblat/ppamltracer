/* tracer.c -- PPAML timing instrumentation
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

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <otf.h>

#include <ppaml/tracer.h>
#include <ppaml/tracer/internal.h>

#include "timing.h"
#include "require.h"

const size_t ppaml_tracer_t_size = sizeof(ppaml_tracer_t);
const size_t ppaml_phase_t_size = sizeof(ppaml_phase_t);

int ppaml_tracer_init(
	ppaml_tracer_t *const tracer,
	const char report_name_base[])
{
	int r = 0;
	// Initialize the file manager.
	const uint32_t max_fds = 1024; // TODO: ulimit default on my system
	tracer->manager = OTF_FileManager_open(max_fds);
	require_nonnull(tracer->manager, 1);
	// Initialize the writer.
	tracer->writer = OTF_Writer_open(report_name_base, 1, tracer->manager);
	require_nonnull(tracer->writer, 2);
	// Record that we have a resolution of 1 microsecond.
	require_one(
		OTF_Writer_writeDefTimerResolution(tracer->writer, 0, 1000000),
		3);
	// Define an OTF process (i.e., a thread).
	require_one(
		OTF_Writer_writeDefProcess(tracer->writer, 0, 1, "main", 0),
		4);
done:	return r;
}

int ppaml_tracer_init_from_env(ppaml_tracer_t *const tracer)
{
	int r = 0;
	// Figure out where to put the trace.
	const char *const report_name_base = getenv("PPAMLTRACER_TRACE_BASE");
	require_nonnull(report_name_base, 5);
	if (report_name_base[0] == '\0') {
		/* The variable exists, but it's empty, so we can't do anything
		 * with it. */
		r = 5;
		goto done;
	}
	// Create the trace.
	r = ppaml_tracer_init(tracer, report_name_base);
done:	return r;
}

int ppaml_tracer_done(ppaml_tracer_t *const tracer)
{
	int r = 0;
	// Close the writer.
	require_one(OTF_Writer_close(tracer->writer), 1);
	tracer->writer = NULL;
	// Close the file manager.
	OTF_FileManager_close(tracer->manager);
	tracer->manager = NULL;
done:	return r;
}

int ppaml_phase_init(
	ppaml_tracer_t *const tracer,
	ppaml_phase_t *const phase,
	const char name[])
{
	static uint32_t next_function_id = 1;
	int r = 0;
	require_one(
		OTF_Writer_writeDefFunction(
			tracer->writer,
			0,	// stream id = 1
			next_function_id,
			name,
			0,	// no function group
			0),	// no source code
		1);
	phase->tracer = tracer;
	phase->otf_function_id = next_function_id;
	next_function_id++;
done:	return r;
}

int ppaml_phase_done(ppaml_phase_t *const phase)
{
	phase->tracer = NULL;
	phase->otf_function_id = 0;
	return 0;
}

int ppaml_phase_start(ppaml_phase_t *const phase)
{
	int r = 0;
	uint64_t now;
	require_zero(timing_get_wall(&now), 1);
	require_one(
		OTF_Writer_writeEnter(
			phase->tracer->writer,
			now,
			phase->otf_function_id,
			1,	// process id = 1
			0),	// no source code
		2);
done:	return r;
}

int ppaml_phase_stop(ppaml_phase_t *const phase)
{
	int r = 0;
	uint64_t now;
	require_zero(timing_get_wall(&now), 1);
	require_one(
		OTF_Writer_writeLeave(
			phase->tracer->writer,
			now,
			phase->otf_function_id,
			1,	// process id = 1
			0),	// no source code
		2);
done:	return r;
}
