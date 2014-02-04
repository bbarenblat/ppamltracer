/* ExplicitlyConfiguredTracer -- tracer initialized with ppaml_tracer_init
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
 * A tracer initialized with an explicit set of configuration parameters.
 */
public final class ExplicitlyConfiguredTracer extends TracerCWrapper {

    /**
     * Base file path for trace report output.
     */
    private final String reportNameBase;

    /**
     * Creates and initializes an <code>ExplicitlyConfiguredTracer</code>.  The
     * trace report will be stored in Open Trace Format.
     *
     * @param reportNameBase all trace file paths will begin with this path.
     */
    public ExplicitlyConfiguredTracer(final String reportNameBase) {
        this.reportNameBase = reportNameBase;
        super.open();
    }

    ////////////////////////// Low-level life cycle  //////////////////////////

    ///// JNI adapter code /////

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

}
