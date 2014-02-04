/* TracerCWrapper -- wrapped ppaml_tracer_t
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
 * Abstract base class for all tracer implementations.
 */
abstract class TracerCWrapper extends CWrapper implements Tracer {

    @Override
    public Phase createPhase(final String name) {
        return new Phase(this, name);
    }


    ////////////////////////// Low-level life cycle  //////////////////////////

    ///// JNI shared library /////

    static {
        System.loadLibrary("ppamltracer_java");
    }

    /**
     * Wraps <code>malloc</code> to allocate a <code>ppaml_tracer_t</code>.
     *
     * @return Same as <code>malloc</code>.
     */
    protected static native long mallocTracer();

    /**
     * Wraps <code>ppaml_tracer_init</code>.
     *
     * @return -1 if converting <code>reportNameBase</code> from a
     * <code>String</code> to a <code>char *</code> failed; all other return
     * codes are the same as <code>ppaml_tracer_init</code>.
     */
    protected static native int ppaml_tracer_init(
        long tracer,
        final String reportNameBase);

    /**
     * Wraps <code>ppaml_tracer_init_from_env</code>.
     *
     * @return Same as <code>ppaml_tracer_init_from_env</code>.
     */
    protected static native int ppaml_tracer_init_from_env(long tracer);

    /**
     * Wraps <code>ppaml_tracer_done</code>.
     *
     * @return Same as <code>ppaml_tracer_done</code>.
     */
    protected static native int ppaml_tracer_done(long tracer);

    /**
     * Wraps <code>free</code>.
     */
    protected static native void freeTracer(long tracer);


    ///// JNI adapter code /////

    @Override
    protected void allocateCPointer() {
        this.cPointer = mallocTracer();
    }

    @Override
    protected abstract void initializeCPointer();

    @Override
    protected void finalizeCPointer() {
        assert this.cPointer != 0;
        final int r = ppaml_tracer_done(this.cPointer);
        if (r != 0) {
            throw new UnexpectedReturnValueError(r);
        }
    }

    @Override
    protected void freeCPointer() {
        assert this.cPointer != 0;
        freeTracer(this.cPointer);
    }

}
