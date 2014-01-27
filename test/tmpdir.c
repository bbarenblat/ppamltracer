/* tmpdir.c -- fixture to create a temporary directory
 * Copyright (C) 2010  caf <https://stackoverflow.com/users/134633>
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
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

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
