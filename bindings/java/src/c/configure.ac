# configure.ac -- autoconf script for ppamltracer-java (JNI wrapper)
# Copyright (C) 2013, 2014  Galois, Inc.
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
AC_INIT([ppamltracer-java], [0.2.0], [bbarenblat@galois.com])

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
