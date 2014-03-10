/* tmpdir.h -- fixture to create a temporary directory
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
