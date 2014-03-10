# otf.m4 -- helper function to detect OTF libraries            -*- Autoconf -*-
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
