/* tmpdir.h -- fixture to create a temporary directory
 * Copyright (C) 2014  Galois, Inc.
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
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622.
 *
 *
 * This fixture creates and cleans a temporary subdirectory of the current
 * directory.  While the subdirectory is active, you can call 'tmpdir_name' to
 * get its name. */

#ifndef PPAML_TRACER_TEST_TMPDIR_H
#define PPAML_TRACER_TEST_TMPDIR_H

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <check.h>

#define HAVE_FIXTURE_tmpdir (HAVE_STRUCT_FTW && HAVE_MKDTEMP && HAVE_NFTW)

#if HAVE_FIXTURE_tmpdir
	void tmpdir_make();
	void tmpdir_clean();

	/* Returns the name of the currently-active temporary directory.  If no
	 * directory is active, returns NULL. */
	const char *tmpdir_name();

	// Registers the fixture.
	inline void tmpdir_add_checked(TCase *const tc) {
		tcase_add_checked_fixture(tc, tmpdir_make, tmpdir_clean);
	}
#endif

#endif
