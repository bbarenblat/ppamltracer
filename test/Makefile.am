# Makefile.am -- automake script for ppamltracer tests
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

TESTS = check_ppamltracer
check_PROGRAMS = check_ppamltracer
check_ppamltracer_SOURCES = \
	check_ppamltracer.c \
	tmpdir.c \
	tmpdir.h \
	trace_invalid.c \
	trace_invalid.h \
	trace_valid.c \
	trace_valid.h
check_ppamltracer_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	@check_CFLAGS@
check_ppamltracer_LDADD = \
	$(top_builddir)/src/libppamltracer.la \
	@check_LIBS@
