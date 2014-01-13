# Makefile.am -- automake script for ppamltracer
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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = include src
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
