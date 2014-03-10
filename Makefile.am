# Makefile.am -- automake script for ppamltracer
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

SUBDIRS = include src test
EXTRA_DIST = Doxyfile bindings examples
dist-hook:
	$(RM) `find $(distdir) -type f -name .gitignore`

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ppamltracer.pc

include $(srcdir)/Makefile_doxygen.am
if DX_COND_doc
MOSTLYCLEANFILES = $(DX_CLEANFILES)

all-local: doc

doc: doxygen-doc

install-data-local: install-doxygen-doc
install-doxygen-doc: $(DX_DOCDIR)/$(DX_PROJECT).tag
	$(INSTALL) -d $(docdir)
	cp -a $(DX_DOCDIR)/* $(docdir)

uninstall-local: uninstall-doxygen-doc
uninstall-doxygen-doc:
	$(RM) -r $(docdir)/{html,latex,man,$(DX_PROJECT).{ps,pdf},rtf,xml}
	$(RM) $(docdir)/$(DX_PROJECT).tag

.PHONY: doc install-doxygen-doc uninstall-doxygen-doc
endif DX_COND_doc
