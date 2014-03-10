# Makefile.am -- automake script for ppamltracer-java (JNI wrapper)
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

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libppamltracer_java.la
libppamltracer_java_la_SOURCES = \
	$(srcdir)/com_galois_ppaml_tracer_Phase.c \
	$(srcdir)/com_galois_ppaml_tracer_Phase.h \
	$(srcdir)/com_galois_ppaml_tracer_TracerCWrapper.c \
	$(srcdir)/com_galois_ppaml_tracer_TracerCWrapper.h \
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
