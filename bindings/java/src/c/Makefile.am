# Makefile.am -- automake script for ppamltracer-java (JNI wrapper)
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

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libppamltracer_java.la
libppamltracer_java_la_SOURCES = \
	$(srcdir)/com_galois_ppaml_tracer_Phase.c \
	$(srcdir)/com_galois_ppaml_tracer_Phase.h \
	$(srcdir)/com_galois_ppaml_tracer_Tracer.c \
	$(srcdir)/com_galois_ppaml_tracer_Tracer.h \
	$(srcdir)/pointer.h
libppamltracer_java_la_CFLAGS = \
	-std=c99 \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-pedantic \
	-I@JAVA_HOME@/include
libppamltracer_java_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-export-symbols-regex '^Java_'
