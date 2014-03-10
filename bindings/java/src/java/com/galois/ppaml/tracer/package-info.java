/* package-info.java -- package information
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

/**
 * A lightweight, portable tracing library for instrumenting generated code.
 *
 * <p> ppamltracer is a lightweight, portable tracing library designed for
 * explicit instrumention of generated code. If you're writing a compiler and
 * need hard data on your optimizer's efficacy, ppamltracer is the library for
 * you.  This Java package provides bindings to the libppamltracer C API.
 *
 * <p> This package's usage can be summed up in a few lines:
 *
 * <pre>
 *   // Create report.
 *   Tracer tracer = new ExplicitlyConfiguredTracer("/tmp/my_report");
 *
 *   // Register phase 1 and do stuff.
 *   Phase phase = tracer.createPhase("phase 1");
 *   phase.start();
 *   doStuff();
 *   phase.stop();
 *   phase.close();
 *
 *   // Register phase 2 and do different stuff.
 *   phase = tracer.createPhase("phase 2");
 *   // Other stuff
 *   phase.start();
 *   doOtherStuff();
 *   phase.stop();
 *   // Yet more stuff
 *   phase.start();
 *   doYetMoreStuff();
 *   phase.stop();
 *
 *   // Clean up.
 *   phase.close();
 *   tracer.close();
 * </pre>
 *
 * <p> This creates a report with the total runtime of <code>doStuff</code>
 * recorded as "phase 1" and the total runtime of <code>doOtherStuff</code> and
 * <code>doYetMoreStuff</code> combined as "phase 2".
 *
 * <p> <code>com.galois.ppaml.tracer.examples.Simple</code>, included in the
 * distribution, contains a more lengthy example.
 *
 * <p> ppamltracer writes trace logs in the
 * <a href="http://tu-dresden.de/zih/otf/">Open Trace Format</a>, a free and
 * open standard developed by the Zentrum für Informationsdienste und
 * Hochleistungsrechnen (Center for Information Services and High-Performance
 * Computing) at the Technical University of Dresden.
 *
 * <p> We developed ppamltracer as part of DARPA's
 * <a href="http://darpa.mil/Our_Work/I2O/Programs/Probabilistic_Programming_for_Advanced_Machine_Learning_(PPAML).aspx">Probabilistic
 * Programming for Advancing Machine Learning</a> (PPAML) project.
 *
 * @author Galois, Inc.
 *
 * @version 0.2.1
 */

package com.galois.ppaml.tracer;
