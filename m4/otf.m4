# otf.m4 -- helper function to detect OTF libraries            -*- Autoconf -*-
# Copyright (C) 2013  Galois, Inc.
#
# This library is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this library.  If not, see <http://www.gnu.org/licenses/>.
#
# To contact Galois, complete the Web form at <http://corp.galois.com/contact/>
# or write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland,
# Oregon, 97204-1622.

# GALOIS_PPAML_LIB_OPENTRACE(ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
# This macro expands the OTF_CFLAGS and OTF_LIBS variables, but it does not add
# them to CFLAGS or LIBS.
AC_DEFUN([GALOIS_PPAML_LIB_OPENTRACE], [
    AC_REQUIRE([AC_PROG_SED])
    otf_ok=whoKnows
    # Check for otfconfig.
    AC_PATH_PROG([OTFCONFIG], [otfconfig])
    AC_ARG_VAR([OTFCONFIG], [path to otfconfig binary])
    if test -z "$OTFCONFIG"; then
        otf_ok=no
    else
        OTF_CFLAGS="`"$OTFCONFIG" --includes`"
        OTF_LIBS="`"$OTFCONFIG" --libs`"
        # Save CFLAGS and LIBS, and reset CFLAGS and LIBS to the OTF
        # requirements.
        SAVECFLAGS="$CFLAGS"
        SAVELIBS="$LIBS"
        CFLAGS="$OTF_CFLAGS"
        LIBS="$OTF_LIBS"
        # Try to compile using the CFLAGS.
        AC_COMPILE_IFELSE(
            [AC_LANG_PROGRAM(
                [#include <otf.h>"],
                [int x = otf_errno;])],
            [otf_compile_headers=yes],
            [otf_compile_headers=no])
        # Try to compile using the libraries.
        AC_LINK_IFELSE(
            [AC_LANG_PROGRAM(
                [#include <otf.h>"],
                [int x = otf_errno;])],
            [otf_link=yes],
            [otf_link=no])
        # Did we pass?
        if test \( "$otf_compile_headers" = "yes" \) -a \
            \( "$otf_link" = "yes" \); then
            otf_ok=yes
        else
            otf_ok=no
        fi
        # Restore CFLAGS and LIBS.
        CFLAGS="$SAVECFLAGS"
        LIBS="$SAVELIBS"
    fi
    # Execute the users' blocks.
    if test "$otf_ok" = "yes"; then
        AC_SUBST([OTF_CFLAGS])
        AC_SUBST([OTF_LIBS])
        ifelse([$1], , :, [$1])
    else
        ifelse([$2], , :, [$2])
    fi
])
