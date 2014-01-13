/* UnexpectedReturnValueError -- unexpected return value from C API
 * Copyright (C) 2014  Galois, Inc.
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at
 * <http://corp.galois.com/contact/> or write to Galois, Inc., 421 Southwest
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

package com.galois.ppaml.tracer;

/**
 * Signals an unexpected return value from libppamltracer.  If this error is
 * thrown, it indicates a bug in ppamltracer, and you should report it to the
 * developers.
 */
public class UnexpectedReturnValueError extends java.lang.Error {

    static final long serialVersionUID = -2476858873119724922L;

    protected UnexpectedReturnValueError(int r) {
        super("unexpected return value " + r + "\n"
              + "This is a bug in ppamltracer!  Report it to the maintainers.");
    }

}
