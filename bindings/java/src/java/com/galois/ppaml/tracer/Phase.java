/* Phase -- wrapped ppaml_phase_t
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
 * A phase of execution to trace.  This class exports no public constructors;
 * to instantiate it, use <code>Tracer.createPhase</code>.
 */
public final class Phase extends CWrapper {

    /**
     * The <code>TracerCWrapper</code> this phase is associated with.
     */
    private final TracerCWrapper tracer;

    /**
     * The name of the phase.
     */
    private final String name;

    /**
     * Creates and initializes a <code>Phase</code>.  You should not call this;
     * call <code>Tracer.createPhase</code> instead.
     *
     * @param tracer the {@link TracerCWrapper} to associate the
     * <code>Phase</code> with.
     *
     * @param name the name of the phase.
     */
    protected Phase(TracerCWrapper tracer, final String name) {
        this.tracer = tracer;
        this.name = name;
        super.open();
    }


    ////////////////////////// Low-level life cycle  //////////////////////////

    ///// JNI shared library /////

    static {
        System.loadLibrary("ppamltracer_java");
    }

    /**
     * Wraps <code>malloc</code> to allocate a <code>ppaml_phase_t</code>.
     *
     * @return Same as <code>malloc</code>.
     */
    private static native long mallocPhase();

    /**
     * Wraps <code>ppaml_phase_init</code>.
     *
     * @return -1 if converting <code>name</code> from a <code>String</code> to
     * a <code>char *</code> failed; all other return codes are the same as
     * <code>ppaml_phase_init</code>.
     */
    private static native int ppaml_phase_init(
        long tracer,
        long phase,
        final String reportNameBase);

    /**
     * Wraps <code>ppaml_phase_done</code>.
     *
     * @return Same as <code>ppaml_phase_done</code>.
     */
    private static native int ppaml_phase_done(long phase);

    /**
     * Wraps <code>free</code>.
     */
    private static native void freePhase(long phase);


    ///// JNI adapter code /////

    @Override
    protected void allocateCPointer() {
        this.cPointer = mallocPhase();
    }

    /**
     * Installs data into the structure to make it ready for use.
     *
     * <p> <b>Precondition:</b> Initialization succeeded.
     *
     * @throws OutOfMemoryError converting <code>name</code> from a
     * <code>String</code> to a <code>char *</code> failed.
     *
     * @throws OTFWriterPhaseDefinitionException defining the OTF phase failed.
     */
    @Override
    protected void initializeCPointer() {
        assert this.cPointer != 0;
        assert tracer != null;
        assert name != null;
        final int r = ppaml_phase_init(tracer.cPointer, this.cPointer, name);
        switch (r) {
        case 0:
            break;
        case -1:
            throw new java.lang.OutOfMemoryError(
                "failed to convert Java string to C string");
        case 1:
            throw new OTFWriterPhaseDefinitionException();
        default:
            throw new UnexpectedReturnValueError(r);
        }
    }

    @Override
    protected void finalizeCPointer() {
        assert this.cPointer != 0;
        final int r = ppaml_phase_done(this.cPointer);
        if (r != 0) {
            throw new UnexpectedReturnValueError(r);
        }
    }

    @Override
    protected void freeCPointer() {
        assert this.cPointer != 0;
        freePhase(this.cPointer);
    }


    ////////////////////////// Starting and stopping //////////////////////////

    ///// JNI shared library /////

    /**
     * Wraps <code>ppaml_phase_start</code>.
     *
     * @return Same as <code>ppaml_phase_start</code>.
     */
    private static native int ppaml_phase_start(long phase);

    /**
     * Wraps <code>ppaml_phase_stop</code>.
     *
     * @return Same as <code>ppaml_phase_stop</code>.
     */
    private static native int ppaml_phase_stop(long phase);


    ///// JNI adapter code /////

    /**
     * Records the start of a <code>Phase</code>.
     *
     * @throws ClosedException the <code>Phase</code> has been closed.
     *
     * @throws ClockAcquisitionException getting the time failed.
     *
     * @throws OTFWriterEntryException recording the phase start failed.
     */
    public void start() {
        if (this.cPointer == 0) {
            throw new ClosedException();
        }
        final int r = ppaml_phase_start(this.cPointer);
        switch (r) {
        case 0:
            break;
        case 1:
            throw new ClockAcquisitionException();
        case 2:
            throw new OTFWriterEntryException();
        default:
            throw new UnexpectedReturnValueError(r);
        }
    }

    /**
     * Records the end of a <code>Phase</code>.
     *
     * @throws ClosedException the <code>Phase</code> has been closed.
     *
     * @throws ClockAcquisitionException getting the time failed.
     *
     * @throws OTFWriterExitException recording the phase stop failed.
     */
    public void stop() {
        if (this.cPointer == 0) {
            throw new ClosedException();
        }
        final int r = ppaml_phase_stop(this.cPointer);
        switch (r) {
        case 0:
            break;
        case 1:
            throw new ClockAcquisitionException();
        case 2:
            throw new OTFWriterExitException();
        default:
            throw new UnexpectedReturnValueError(r);
        }
    }

}
