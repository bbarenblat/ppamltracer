/* EnvironmentConfiguredTracer -- tracer initialized with ppaml_tracer_init_from_env
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
 * A tracer initialized automatically from environment variables.
 *
 * <p>The currently-supported environment variables are:
 *
 * <dl>
 *   <dt><code>PPAMLTRACER_TRACE_BASE</code></dt><dd>All trace file paths will
 *     begin with this path.</dd>
 */
public final class EnvironmentConfiguredTracer extends TracerCWrapper {

    /**
     * Creates and initializes an <code>EnvironmentConfiguredTracer</code>.  The
     * trace report will be stored in Open Trace Format.
     */
    public EnvironmentConfiguredTracer() {
        super.open();
    }

    ////////////////////////// Low-level life cycle  //////////////////////////

    ///// JNI adapter code /////

    /**
     * Installs data into the structure to make it ready for use.
     *
     * <p> <b>Precondition:</b> Initialization succeeded.
     *
     * @throws OTFManagerInitializationException the Open Trace Format file
     * manager could not be initialized.
     *
     * @throws OTFWriterInitializationException the Open Trace Format writer
     * could not be initialized.
     *
     * @throws OTFWriterResolutionException setting the trace resolution failed.
     *
     * @throws OTFWriterProcessDefinitionException defining the main OTF process
     * failed.
     */
    @Override
    protected void initializeCPointer() {
        assert this.cPointer != 0;
        final int r = ppaml_tracer_init_from_env(this.cPointer);
        switch (r) {
        case 0:
            break;
        case 1:
            throw new OTFManagerInitializationException();
        case 2:
            throw new OTFWriterInitializationException();
        case 3:
            throw new OTFWriterResolutionException();
        case 4:
            throw new OTFWriterProcessDefinitionException("main");
        case 5:
            throw new UndefinedTracerBasePathException();
        default:
            throw new UnexpectedReturnValueError(r);
        }
    }

}
