/* trace_valid.c -- fixture to create an valid OTF trace
 * Copyright (C) 2014  Galois, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at
 * <http://corp.galois.com/contact/> or write to Galois, Inc., 421 Southwest
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

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
