/* Tracer -- Java analogue to ppaml_tracer_t
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
 * State bundle for ppamltracer.
 *
 * <p> ppamltracer is fundamentally a set of stateful operations; an object
 * implementing <code>Tracer</code> holds the state ppamltracer needs to operate
 * properly.
 */
public interface Tracer {

    /**
     * Creates and initializes a {@link Phase} associated with this
     * <code>Tracer</code>.
     *
     * @param name the name of the phase.
     */
    public Phase createPhase(final String name);

    /**
     * Finalizes and frees the underlying structure.
     */
    public void close();

}
