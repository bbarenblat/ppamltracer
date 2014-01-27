/* trace_valid.h -- fixture to create an valid trace
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
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622.
 *
 *
 * This fixture creates an valid OTF trace in the currently-active temporary
 * directory.  It does not remove it, as it will be removed when the temporary
 * directory is cleaned.
 *
 * Runtime dependency: tmpdir */

#ifndef PPAML_TRACER_TEST_TRACE_VALID_H
#define PPAML_TRACER_TEST_TRACE_VALID_H

#include "tmpdir.h"

#define HAVE_FIXTURE_trace_valid HAVE_FIXTURE_tmpdir

#if HAVE_FIXTURE_trace_valid
	void trace_valid_make();

	/* Despite its name, this function does not actually clean anything.
	 * It's provided to make it easier to interface with Check. */
	void trace_valid_clean();

	/* Returns the path to the trace.  If no trace is active, returns
	 * NULL. */
	const char *trace_valid_path();

	// Registers the fixture.
	inline void trace_valid_add_checked(TCase *const tc) {
		tcase_add_checked_fixture(
			tc,
			trace_valid_make,
			trace_valid_clean);
	}
#endif

#endif
