# configure.ac -- autoconf script for ppamltracer
# Copyright (C) 2013, 2014  Galois, Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#   1. Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#   3. Neither Galois's name nor the names of other contributors may be used to
#      endorse or promote products derived from this software without specific
#      prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Fire up Autoconf.
AC_PREREQ([2.69])
AC_INIT([ppamltracer], [0.2.1], [bbarenblat@galois.com])

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
AM_PROG_CC_C_O
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
