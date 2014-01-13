/* tracer.c -- PPAML timing instrumentation
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

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <stdint.h>
#include <stdlib.h>

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
