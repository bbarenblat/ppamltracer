# configure.ac -- autoconf script for ppamltracer
# Copyright (C) 2013  Galois, Inc.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.
#
# To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
# or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
# Oregon, 97204-1622.

# Fire up Autoconf.
AC_PREREQ([2.69])
AC_INIT([ppamltracer], [0.1.0], [bbarenblat@galois.com])

# Fire up Automake.  Use ustar format because we actually have file names
# longer than 99 characters (hooray, Java).
AM_INIT_AUTOMAKE([-Wall -Wno-gnu -Werror foreign tar-ustar no-dist-gzip \
	dist-bzip2])
AC_CONFIG_MACRO_DIR([m4])

# Build quietly.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Build a C library.
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AC_PROG_CC_C99
# Automake 1.12 seems to require this, but automake 1.11 doesn't recognize it.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

# Dependencies
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_TYPE([clockid_t])
GALOIS_PPAML_LIB_OPENTRACE(
	[],
	[AC_MSG_ERROR([both OTF development libraries and the 'otfconfig' program are required.  You can install them from <http://tu-dresden.de/die_tu_dresden/zentrale_einrichtungen/zih/forschung/projekte/otf>.])])

# Documentation
DX_HTML_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN([ppamltracer], [Doxyfile], [doc])

# Tests
AC_CHECK_HEADERS([ftw.h unistd.h sys/stat.h time.h])
AC_CHECK_TYPES(
	[struct FTW],
	[],
	[],
	[[#define _XOPEN_SOURCE 500
#include <ftw.h>]])
AC_CHECK_TYPES(
	[struct timespec],
	[],
	[],
	[[#define _POSIX_C_SOURCE 199309L
#include <time.h>]])
AC_CHECK_FUNCS([mkdtemp nanosleep nftw setenv unsetenv])
PKG_CHECK_MODULES([check], [check >= 0.9.4],
	[],
	[AC_MSG_WARN([check not found; `make check' will fail])])
AC_SUBST([check_CFLAGS])
AC_SUBST([check_LIBS])

# Generate Makefiles and pkg-config file.
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	include/Makefile
	test/Makefile])
AC_CONFIG_FILES([ppamltracer.pc])

# All done.
AC_OUTPUT
