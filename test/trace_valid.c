/* trace_valid.c -- fixture to create an valid OTF trace
 * Copyright (C) 2014  Galois, Inc.
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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <check.h>

#include <ppaml/tracer.h>

#include "trace_valid.h"
#include "tmpdir.h"

#if HAVE_FIXTURE_tmpdir

	static const char name[] = "trace";
	static bool active = false;

	void trace_valid_make()
	{
		active = true;
		ppaml_tracer_t tracer;
		ck_assert(
			ppaml_tracer_init(&tracer, trace_valid_path()) == 0);
		ppaml_phase_t phase;
		ck_assert(
			ppaml_phase_init(&tracer, &phase, "test phase") == 0);
		ck_assert(ppaml_phase_start(&phase) == 0);
		ck_assert(ppaml_phase_stop(&phase) == 0);
		ck_assert(ppaml_phase_done(&phase) == 0);
		ck_assert(ppaml_tracer_done(&tracer) == 0);
	}

	const char *trace_valid_path()
	{
		static char path[128];
		if (active) {
			strcpy(path, tmpdir_name());
			strcat(path, "/");
			strcat(path, name);
			return path;
		} else {
			return NULL;
		}

	}

	void trace_valid_clean()
	{
		active = false;
	}

#endif
