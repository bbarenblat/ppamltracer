# ppamltracer -- Python bindings to ppamltracer
# Copyright (C) 2013, 2014  Galois, Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#   1. Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#   3. Neither Galois's name nor the names of other contributors may be used to
#      endorse or promote products derived from this software without specific
#      prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""A tracing library for explicit instrumentation of generated code.

ppamltracer is a lightweight, portable tracing library designed for
explicit instrumention of generated code.  If you're writing a compiler
and need hard data on your optimizer's efficacy, ppamltracer is the
library for you.  ppamltracer-python provides a high-level Python API on
top of the libppamltracer C API.

ppamltracer-python's usage can be summed up in a couple lines:

    from ppamltracer import Tracer
    with Tracer("/tmp/my_report") as tracer:
        with tracer.create_phase("phase 1") as phase:
            with phase.running():
                do_stuff()
        with tracer.create_phase("phase 2") as phase:
            with phase.running():
                do_other_stuff()
            with phase.running():
                do_yet_more_stuff()

This creates a report with the total runtime of do_stuff recorded as
"phase 1", and the total runtime of do_other_stuff and do_yet_more_stuff
combined as "phase 2".

The ppamltracer-python distribution also contains a more lengthy example
in the "examples" directory.

ppamltracer writes trace logs in the Open Trace Format [1], a free and
open standard developed by the Zentrum fuer Informationsdienste und
Hochleistungsrechnen (Center for Information Services and
High-Performance Computing) at the Technical University of Dresden.

We developed ppamltracer and ppamltracer-python as part of DARPA's
Probabilistic Programming for Advancing Machine Learning (PPAML)
project [2].

References:
    [1] http://tu-dresden.de/zih/otf/
    [2] http://darpa.mil/Our_Work/I2O/Programs/Probabilistic_Programming_for_Advanced_Machine_Learning_(PPAML).aspx
"""
# TODO: Replace "ue" in "fuer" with Unicode u-with-diaresis (blocking on
# deployment of fix to Python #1065986, "Fix pydoc crashing on unicode
# strings").

import ctypes
import ctypes.util
import warnings


################################ Low-level API ################################

_lib = ctypes.cdll.LoadLibrary(ctypes.util.find_library("ppamltracer"))
"""ctypes binding to the ppamltracer C library."""

# The code in this section binds the C API using ctypes, translating
# return codes into the exception hierarchy defined below.  Since these
# functions are not intended to be used externally, they're not
# documented using pydoc; see the ppamltracer C API documentation for
# information on their behavior.

_sizeof_ppaml_tracer_t = \
    ctypes.c_size_t.in_dll(_lib, "ppaml_tracer_t_size").value

_sizeof_ppaml_phase_t = ctypes.c_size_t.in_dll(_lib, "ppaml_phase_t_size").value

def _ppaml_tracer_init(tracer, report_name_base):
    if report_name_base is None:
        _initialize = _lib.ppaml_tracer_init_from_env
        _initialize_args = [tracer]
    else:
        _initialize = _lib.ppaml_tracer_init
        _initialize_args = [tracer, report_name_base]
    result = _initialize(*_initialize_args)
    if result == 0:
        return None
    elif result == 1:
        raise OTFManagerInitializationError()
    elif result == 2:
        raise OTFWriterInitializationError()
    elif result == 3:
        raise OTFWriterResolutionError()
    elif result == 4:
        raise OTFWriterProcessDefinitionError("main")
    elif result == 5:
        raise UndefinedTracerBasePathError()
    else:
        _warn_unexpected_return_code()
        raise TracerError()

def _ppaml_tracer_done(tracer):
    result = _lib.ppaml_tracer_done(tracer)
    if result == 0:
        return None
    elif result == 1:
        raise OTFWriterCloseError()
    else:
        _warn_unexpected_return_code()
        raise TracerError()

def _ppaml_phase_init(tracer, phase, name):
    result = _lib.ppaml_phase_init(tracer, phase, name)
    if result == 0:
        return None
    elif result == 1:
        raise OTFWriterPhaseDefinitionError()
    else:
        _warn_unexpected_return_code()
        raise TracerError()

def _ppaml_phase_start(phase):
    result = _lib.ppaml_phase_start(phase)
    if result == 0:
        return None
    elif result == 1:
        raise ClockAcquisitionError()
    elif result == 2:
        raise OTFWriterEntryError()
    else:
        _warn_unexpected_return_code()
        raise TracerError()

def _ppaml_phase_stop(phase):
    result = _lib.ppaml_phase_stop(phase)
    if result == 0:
        return None
    elif result == 1:
        raise ClockAcquisitionError()
    elif result == 2:
        raise OTFWriterExitError()
    else:
        _warn_unexpected_return_code()
        raise TracerError()

def _ppaml_phase_done(phase):
    result = _lib.ppaml_phase_done(phase)
    if result == 0:
        return None
    else:
        _warn_unexpected_return_code()
        raise TracerError()


############################# Resource-based API ##############################

class Tracer(object):
    """A tracer for programs, which records execution timing information.

    This class is designed to be used with Python's "with" statement-- e.g.,

        with Tracer("/tmp/my_report") as tracer:
            main(tracer)

    """

    def __init__(self, report_name_base=None):
        """Create a new tracer.

        The tracer will create an Open Trace Format report during program
        execution.  If report_name_base is specified, the multiple files of the
        report will all start with report_name_base; otherwise, they will start
        with the contents of the PPAMLTRACER_TRACE_BASE environment variable.

        """
        self._report_name_base = report_name_base
        self._underlying = None

    def __enter__(self):
        if self._underlying is None:
            self._underlying = \
                ctypes.create_string_buffer(_sizeof_ppaml_tracer_t)
            _ppaml_tracer_init(self._underlying, self._report_name_base)
        return self

    def __exit__(self, *exception_info):
        if self._underlying is not None:
            _ppaml_tracer_done(self._underlying)
            self._underlying = None

    def create_phase(self, name):
        """Construct a Phase associated with this Tracer.

        This function is merely a convenience function; internally, it shells
        out to the Phase constructor.

        """
        return Phase(self, name)


class Phase(object):
    """A phase of computation traced by ppamltracer.

    This class is designed to be used with Python's "with" statement-- e.g.,

        with Phase(tracer, "my phase") as phase:
            with phase.running():
                do_stuff()
            with phase.running():
                do_stuff_again()

    Note the double use of "with".  The outer "with" manages the lifetime of
    a Phase; the inner "with" actually starts and stops the phase timer.

    """

    def __init__(self, tracer, name):
        """Define a new phase tracked by a given tracer."""
        self._tracer = tracer
        self._name = name
        self._underlying = None

    def __enter__(self):
        if self._underlying is None:
            self._underlying = \
                ctypes.create_string_buffer(_sizeof_ppaml_phase_t)
            _ppaml_phase_init(
                self._tracer._underlying,
                self._underlying,
                self._name)
        return self

    def __exit__(self, *exception_info):
        if self._underlying is not None:
            _ppaml_phase_done(self._underlying)
            self._underlying = None

    def _start(self):
        _ppaml_phase_start(self._underlying)

    def _stop(self):
        _ppaml_phase_stop(self._underlying)

    def running(self):
        """ Create a resource manager for the phase timer.

        This manager handles starting and stopping the phase, allowing you to
        time operations by saying, e.g.,

            with phase.running():
                do_stuff()

        """
        return _PhaseTimerManager(self)


class _PhaseTimerManager(object):
    """Manage entry and exit from a Phase using the "with" statement."""

    def __init__(self, phase):
        self._phase = phase

    def __enter__(self):
        self._phase._start()

    def __exit__(self, *exception_info):
        self._phase._stop()


############################# Exception hierarchy #############################

class TracerError(Exception):
    """A generic ppamltracer error."""
    pass

class ConfigurationError(TracerError):
    """An error related to tracer configuration."""
    pass

class UndefinedTracerBasePathError(ConfigurationError):
    """Signals an attempt to initialize a tracer with an undefined basepath.

    """

    def __init__(self):
        super(UndefinedTracerBasePathError, self).__init__(
            "attempted to configure tracer with undefined base path")

class OTFError(TracerError):
    """An error related to Open Trace Format input and output."""
    pass

class OTFManagerError(OTFError):
    """An error caused by the Open Trace Format manager."""
    pass

class OTFManagerInitializationError(OTFManagerError):
    """Failure to initialize the Open Trace Format manager."""

    def __init__(self):
        super(OTFManagerInitializationError, self).__init__(
            "could not initialize Open Trace Format file manager")

class OTFWriterError(OTFError):
    """An error caused by the Open Trace Format writer."""
    pass

class OTFWriterInitializationError(OTFWriterError):
    """Failure to initialize the Open Trace Format writer."""

    def __init__(self):
        super(OTFWriterInitializationError, self).__init__(
            "could not open Open Trace Format writer")

class OTFWriterPhaseDefinitionError(OTFWriterError):
    """Failure to define a phase. """

    def __init__(self):
        super(OTFWriterPhaseDefinitionError, self).__init__(
            "could not define phase")

class OTFWriterEntryError(OTFWriterError):
    """Failure to record entry into a phase."""

    def __init__(self):
        super(OTFWriterEntryError, self).__init__(
            "could not record phase start")

class OTFWriterExitError(OTFWriterError):
    """Failure to record exit from a phase."""

    def __init__(self):
        super(OTFWriterExitError, self).__init__("could not record phase end")

class OTFWriterCloseError(OTFWriterError):
    """Failure to close the Open Trace Format writer."""

    def __init__(self):
        super(OTFWriterCloseError, self).__init__(
            "could not close Open Trace Format writer")

class OTFWriterResolutionError(OTFWriterError):
    """Failure to set the tracer resolution."""

    def __init__(self):
        super(OTFWriterResolutionError, self).__init__(
            "could not set trace resolution")

class OTFWriterProcessDefinitionError(OTFWriterError):
    """Failure to define an Open Trace Format process."""

    def __init__(self, process_name=None):
        if process_name is None:
            super(OTFWriterProcessDefinitionError, self).__init__(
                "could not define Open Trace Format process")
        else:
            super(OTFWriterProcessDefinitionError, self).__init__(
                "could not define Open Trace Format process \""
                + process_name
                + "\"")

class TimingError(TracerError):
    """An error related to system timers."""
    pass

class ClockAcquisitionError(TimingError):
    """A failure to get the current clock time."""

    def __init__(self):
        super(ClockAcquisitionError, self).__init__(
            "could not get current time")


############################### Warnings ###############################

class UnwrappedCErrorWarning(Warning):
    """A warning indicating a failure to correctly wrap a C API."""
    pass

def _warn_unexpected_return_code():
    warnings.warn(("Unexpected C return code\n"
                   + "*** This is a bug in ppamltracer-python!  Report it to the maintainers."),
                  UnwrappedCErrorWarning,
                  2)
