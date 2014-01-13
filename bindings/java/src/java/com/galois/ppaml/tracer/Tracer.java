/* Tracer -- wrapped ppaml_tracer_t
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
 * State class for ppamltracer.
 *
 * <p> ppamltracer is fundamentally a set of stateful operations; a
 * <code>Tracer</code> object holds the state ppamltracer needs to operate
 * properly.
 */
public final class Tracer extends CWrapper {

    /**
     * Base file path for trace report output.
     */
    private final String reportNameBase;

    /**
     * Creates and initializes a <code>Tracer</code>.  The trace report will be
     * stored in Open Trace Format.
     *
     * @param reportNameBase all trace file paths will begin with this path.
     */
    public Tracer(final String reportNameBase) {
        this.reportNameBase = reportNameBase;
        super.open();
    }

    /**
     * Creates and initializes a {@link Phase} associated with this
     * <code>Tracer</code>.
     *
     * @param name the name of the phase.
     */
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
    private static native long mallocTracer();

    /**
     * Wraps <code>ppaml_tracer_init</code>.
     *
     * @return -1 if converting <code>reportNameBase</code> from a
     * <code>String</code> to a <code>char *</code> failed; all other return
     * codes are the same as <code>ppaml_tracer_init</code>.
     */
    private static native int ppaml_tracer_init(
        long tracer,
        final String reportNameBase);

    /**
     * Wraps <code>ppaml_tracer_done</code>.
     *
     * @return Same as <code>ppaml_tracer_done</code>.
     */
    private static native int ppaml_tracer_done(long tracer);

    /**
     * Wraps <code>free</code>.
     */
    private static native void freeTracer(long tracer);


    ///// JNI adapter code /////

    @Override
    protected void allocateCPointer() {
        this.cPointer = mallocTracer();
    }

    /**
     * Installs data into the structure to make it ready for use.
     *
     * <p> <b>Precondition:</b> Initialization succeeded.
     *
     * @throws OutOfMemoryError converting <code>reportNameBase</code> from a
     * <code>String</code> to a <code>char *</code> failed.
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
        assert reportNameBase != null;
        final int r = ppaml_tracer_init(this.cPointer, reportNameBase);
        switch (r) {
        case 0:
            break;
        case -1:
            throw new java.lang.OutOfMemoryError(
                "failed to convert Java string to C string");
        case 1:
            throw new OTFManagerInitializationException();
        case 2:
            throw new OTFWriterInitializationException();
        case 3:
            throw new OTFWriterResolutionException();
        case 4:
            throw new OTFWriterProcessDefinitionException("main");
        default:
            throw new UnexpectedReturnValueError(r);
        }
    }

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
