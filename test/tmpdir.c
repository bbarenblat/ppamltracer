/* tmpdir.c -- fixture to create a temporary directory
 * Copyright (C) 2010  caf <https://stackoverflow.com/users/134633>
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

#define _POSIX_C_SOURCE 200809L	// mkdtemp(3)

/* This is a bit out of order, but it needs to go here, before *any* includes
 * are done, so that they can catch _XOPEN_SOURCE. */
#if HAVE_FTW_H
#	define _XOPEN_SOURCE 500
#	include <ftw.h>
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_SYS_STAT_H
#	include <sys/stat.h>
#endif

#if HAVE_UNISTD_H
#	include <unistd.h>
#endif

#include <check.h>

#include "tmpdir.h"

#define ignore(x) ((void)(x))

#if (HAVE_STRUCT_FTW && HAVE_MKDTEMP && HAVE_NFTW)
	static int unlink_cb(
		const char *fpath,
		const struct stat *sb,
		int typeflag,
		struct FTW *ftwbuf)
	{
		ignore(sb);
		ignore(typeflag);
		ignore(ftwbuf);
		return remove(fpath);
	}

	static inline int rm_rf(char *path)
	{
		return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
	}

	static char tmpdir_buf[64];

	void tmpdir_make() {
		const char template[] = "scratch.XXXXXX";
		strcpy(tmpdir_buf, template);
		ck_assert(mkdtemp(tmpdir_buf) != NULL);
	}

	const char *tmpdir_name() {
		if (strlen(tmpdir_buf) == 0) {
			return NULL;
		} else {
			return tmpdir_buf;
		}
	}

	void tmpdir_clean() {
		rm_rf(tmpdir_buf);
		tmpdir_buf[0] = '\0';
	}
#endif
