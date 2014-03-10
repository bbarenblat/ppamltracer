# configure.ac -- autoconf script for ppamltracer-java (JNI wrapper)
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
AC_INIT([ppamltracer-java], [0.2.1], [bbarenblat@galois.com])

# Fire up Automake.
AM_INIT_AUTOMAKE([-Wall -Wno-gnu -Werror foreign no-dist-gzip dist-bzip2])
AC_CONFIG_MACRO_DIR([m4])

# Build quietly.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Build a C library.
AC_PROG_CC
AC_PROG_CC_C99
# Automake 1.12 seems to require this, but automake 1.11 doesn't recognize it.
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

# JNI
AC_ARG_VAR([JAVA_HOME], [path to JDK installation])
if test -z "$JAVA_HOME"; then
    AC_MSG_ERROR([empty JAVA_HOME environment variable; please set it to the path to your JDK installation])
fi
AC_SUBST([JAVA_HOME])

# ppamltracer
AC_CHECK_HEADER([ppaml/tracer.h],
    [],
    [AC_MSG_ERROR([ppamltracer development headers are required (are they in C_INCLUDE_PATH?)])])
AC_CHECK_LIB([ppamltracer], [ppaml_tracer_init],
    [],
    [AC_MSG_ERROR([libppamltracer is required (is it in LIBRARY_PATH?)])])

# Generate Makefile.
AC_CONFIG_FILES([Makefile])

# All done.
AC_OUTPUT
