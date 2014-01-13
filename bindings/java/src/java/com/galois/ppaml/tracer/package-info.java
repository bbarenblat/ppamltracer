/* package-info.java -- package information
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
 *   Tracer tracer = new Tracer("/tmp/my_report");
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
 * @version 0.1.0
 */

package com.galois.ppaml.tracer;
