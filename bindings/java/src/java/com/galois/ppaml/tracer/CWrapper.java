/* CWrapper -- wrapping C structures
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
 * A class which wraps a C structure and associated functions.
 *
 * <p> This class is intended to work hand-in-hand with Java Native Interface
 * code that follows a four-stage lifecycle:
 *
 * <dl>
 *   <dt>Allocation</dt><dd>The system reserves memory and resources needed by
 *     the structure.</dd>
 *   <dt>Initialization</dt><dd>The system installs data into the structure to
 *     make it ready for use.</dd>
 *   <dt>Finalization</dt><dd>The system cleans up the data structure under the
 *     assumption that it will not be used anymore.</dd>
 *   <dt>Freeing</dt><dd>The system releases memory and resources needed by the
 *     structure.</dd>
 * </dl>
 *
 * This class defines abstract methods associated with each of these phases; by
 * defining contracts on them, it can then consolidate the phases into two
 * concretely-implemented operations:
 *
 * <dl>
 *   <dt>Open</dt><dd>The system allocates and initializes the underlying
 *     structure.</dd>
 *   <dt>Close</dt><dd>The system finalizes and frees the underlying
 *     structure.</dd>
 * </dl>
 *
 * <p> <b>This class is not intended to be instantiated directly.  Only
 * instantiate its concrete subclasses.
 */
abstract class CWrapper {

    /**
     * C pointer to the underlying data structure.
     */
    protected long cPointer;


    /////////////////////////// Low-level life cycle ///////////////////////////

    /**
     * Reserves memory and resources needed by the structure.
     *
     * <p> <b>Precondition:</b> <code>cPointer</code> is <code>0</code>.
     *
     * <p> <b>Postcondition:</b> If allocation was successful,
     * <code>cPointer</code> is not <code>0</code>.
     */
    abstract protected void allocateCPointer();

    /**
     * Installs data into the structure to make it ready for use.
     *
     * <p> <b>Precondition:</b> Allocation succeeded.
     */
    abstract protected void initializeCPointer();

    /**
     * Cleans up the structure under the assumption that it will not be used
     * anymore.
     *
     * <p> <b>Precondition:</b> Initialization succeeded.
     */
    abstract protected void finalizeCPointer();

    /**
     * Releases memory and resources needed by the structure.
     *
     * <p> <b>Precondition:</b> <code>cPointer</code> is not <code>0</code>. <br>
     * <b>Precondition:</b> Finalization succeeded.
     *
     * <p> <b>Postcondition:</b> <code>cPointer</code> is <code>0</code>.
     */
    abstract protected void freeCPointer();


    ////////////////////////// High-level life cycle  //////////////////////////

    /**
     * Allocates and initializes the underlying structure.
     *
     * @throws OpenedException the underlying structure has already been opened.
     *
     * @throws NativeOutOfMemoryError allocation failed.
     */
    protected synchronized void open() {
        if (cPointer != 0) {
            throw new OpenedException();
        }
        allocateCPointer();
        if (cPointer == 0) {
            throw new NativeOutOfMemoryError();
        }
        initializeCPointer();
    }

    /**
     * Finalizes and frees the underlying structure.
     *
     * @throws ClosedException the underlying structure has already been closed.
     */
    public synchronized void close() {
        if (cPointer == 0) {
            throw new ClosedException();
        }
        finalizeCPointer();
        freeCPointer();
        cPointer = 0;
    }


    ////////////////////////// Associated exceptions  //////////////////////////

    /**
     * Signals that the underlying structure is already open.
     */
    public class OpenedException extends java.lang.IllegalStateException {

        static final long serialVersionUID = -7226578680647930527L;

        /**
         * Constructs a <code>CWrapper.OpenedException</code> with no detail
         * message.
         */
        protected OpenedException() {
            super();
        }

    }

    /**
     * Signals that the underlying structure is already closed.
     */
    public class ClosedException extends java.lang.IllegalStateException {

        static final long serialVersionUID = -2471059099211958977L;

        /**
         * Constructs a <code>CWrapper.ClosedException</code> with no detail
         * message.
         */
        protected ClosedException() {
            super();
        }

    }


    ////////////////////////////// Miscellaneous  //////////////////////////////

    @Override
    protected void finalize() {
        close();
    }

    @Override
    protected Object clone() throws java.lang.CloneNotSupportedException {
        throw new java.lang.CloneNotSupportedException();
    }

}
