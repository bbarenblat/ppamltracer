/* TracerCWrapper -- wrapped ppaml_tracer_t
 * Copyright (C) 2014  Galois, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither Galois's name nor the names of other contributors may be used to
 *      endorse or promote products derived from this software without specific
 *      prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

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
