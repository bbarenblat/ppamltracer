{- PPAML.Tracer -- PPAML timing instrumentation system (Haskell bindings)
Copyright (C) 2014  Galois, Inc.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
  3. Neither Galois's name nor the names of other contributors may be used to
     endorse or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY GALOIS AND OTHER CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.  IN NO EVENT SHALL GALOIS OR OTHER CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. -}

{-| Getting started with ppamltracer is easy.  Because of Haskell’s intrinsic
laziness, though, getting meaningful results out of ppamltracer is slightly more
difficult.  For instance, consider the following (correctly written) example:

> import PPAML.Tracer
>
> main =
>   withTracer "/tmp/my_report" $ \tracer ->
>     withPhase tracer "phase 1" $ \phase ->
>       withPhaseRunning phase doStuff
>     withPhase tracer "phase 2" $ \phase -> do
>       withPhaseRunning phase doOtherStuff
>       withPhaseRunning phase doYetMoreStuff

This creates a report which appears to record the total runtime of 'doStuff'
recorded as \"phase 1\" and the total runtime of 'doOtherStuff' and
'doYetMoreStuff' combined as \"phase 2\".  In actuality, however, the report
does not record the total runtime – instead,

> withPhaseRunning phase f

records the time required to evaluate 'f'’s result to normal form.  Thus, 'f'
can leverage infinite data as intermediate values; only 'f'’s final result need
be finite. -}

{-# LANGUAGE EmptyDataDecls #-}
{-# LANGUAGE LambdaCase #-}
module PPAML.Tracer
       ( -- * Tracers
         Tracer, TracerHandle
       , withTracer, withTracer'
         -- * Phases
       , Phase, PhaseHandle
       , withPhase
         -- * Timing
       , withPhaseRunning, withPhaseRunning_
         -- * Exceptions
         {-| ppamltracer defines a fairly detailed exception hierarchy; however,
         all exceptions are truly exceptional cases, and they likely will not
         occur in normal use.  For reference, the hierarchy looks like this:

         @
'Control.Exception.SomeException'
└─ 'TracerException'
   ├─ 'OTFException'
   │  ├─ 'OTFManagerException'
   │  │  └─ 'OTFManagerInitializationFailure'
   │  └─ 'OTFWriterException'
   │     ├─ 'OTFWriterInitializationFailure'
   │     ├─ 'OTFPhaseDefinitionFailure'
   │     ├─ 'OTFPhaseEntryFailure'
   │     ├─ 'OTFPhaseExitFailure'
   │     ├─ 'OTFWriterCloseFailure'
   │     ├─ 'OTFTraceResolutionFailure'
   │     └─ 'OTFProcessDefinitionFailure'
   └─ 'TimingException'
      └─ 'ClockAcquisitionFailure'
         @ -}
       , TracerException
         -- ** Open Trace Format exceptions
       , OTFException
       , OTFManagerException, OTFManagerInitializationFailure
       , OTFWriterException, OTFWriterInitializationFailure,
         OTFPhaseDefinitionFailure, OTFPhaseEntryFailure, OTFPhaseExitFailure,
         OTFWriterCloseFailure, OTFTraceResolutionFailure,
         OTFProcessDefinitionFailure
       , TraceBaseUndefined
         -- ** Timing exceptions
       , TimingException, ClockAcquisitionFailure
       ) where

import Control.DeepSeq (NFData, ($!!))
import Control.Exception (bracket_, throw)
import Foreign (Storable, Ptr, allocaBytes, peek)
import Foreign.C (CInt(CInt), CSize(CSize), CString, withCString)
import System.IO.Unsafe (unsafeDupablePerformIO)

import PPAML.Tracer.Exception


-------------------------------- High-level API --------------------------------

{-| Constructs a 'Tracer' and executes the specified computation.  The trace
report will be stored in Open Trace Format; all trace file paths will begin with
the specified path.

Throws:

  * 'OTFManagerInitializationFailure' if the Open Trace Format manager could not
    be initialized.

  * 'OTFWriterInitializationFailure' if the Open Trace Format writer could not
    be initialized.

  * 'OTFTraceResolutionFailure' if setting the trace resolution failed.

  * 'OTFProcessDefinitionFailure' if defining the main OTF process failed.

  * 'OTFWriterCloseFailure' if the Open Trace Format writer could not be closed
    after the computation ran. -}

withTracer :: FilePath -> (TracerHandle -> IO a) -> IO a
withTracer reportBaseName f =
  allocaBytes tracerSize $ \tracerHandle ->
    bracket_ (tracerInit tracerHandle reportBaseName)
             (tracerDone tracerHandle)
             (f tracerHandle)

{-| Similar to 'withTracer'.  Constructs a 'Tracer' and executes the specified
computation.  The trace report will be stored in Open Trace Format; all trace
file paths will begin with the path contained in the environment variable
'PPAMLTRACER_TRACE_BASE'.

Throws:

  * 'OTFManagerInitializationFailure' if the Open Trace Format manager could not
    be initialized.

  * 'OTFWriterInitializationFailure' if the Open Trace Format writer could not
    be initialized.

  * 'OTFTraceResolutionFailure' if setting the trace resolution failed.

  * 'OTFProcessDefinitionFailure' if defining the main OTF process failed.

  * 'OTFWriterCloseFailure' if the Open Trace Format writer could not be closed
    after the computation ran.

  * 'TraceBaseUndefined' if the 'PPAMLTRACER_TRACE_BASE' environment variable is
    undefined or empty. -}

withTracer' :: (TracerHandle -> IO a) -> IO a
withTracer' f =
  allocaBytes tracerSize $ \tracerHandle ->
    bracket_ (tracerInitFromEnv tracerHandle)
             (tracerDone tracerHandle)
             (f tracerHandle)

{-| Constructs a 'Phase' and executes the specified computation.

Throws 'OTFPhaseDefinitionFailure' if the phase could not be defined. -}
withPhase :: TracerHandle       -- ^ the associated 'Tracer'
             -> String          -- ^ the name of the 'Phase'
             -> (PhaseHandle -> IO a)
             -> IO a
withPhase tracer name f =
  allocaBytes phaseSize $ \phaseHandle ->
    bracket_ (phaseInit tracer phaseHandle name)
             (phaseDone phaseHandle)
             (f phaseHandle)

{-| Executes an IO action and evaluates its result to normal form, recording the
time required to do so in the provided 'Phase'. -}
withPhaseRunning :: NFData a => PhaseHandle -> IO a -> IO a
withPhaseRunning phase f = withPhaseRunning_ phase ((return $!!) =<< f)

{-| Executes an IO action, recording the time required to do so in the provided
'Phase'.  Unlinke 'withPhaseRunning', this function does /not/ evaluate the
action’s result.  Use this function when you need finer-grained control over the
precise degree of evaluation than 'withPhaseRunning' offers.
-}
withPhaseRunning_ :: PhaseHandle -> IO a -> IO a
withPhaseRunning_ phase = bracket_ (phaseStart phase) (phaseStop phase)


----------------------------------- Tracers ------------------------------------

----- Data type -----

{-| Tracer state bundle.  ppamltracer is fundamentally a set of stateful
operations; this data type dsecribes the state ppamltracer needs to operate
properly.  A one-to-one mapping exists between 'Tracer's and trace reports, so
you will likely only need one 'Tracer' per program. -}
data Tracer

foreign import ccall unsafe "ppaml/tracer/internal.h & ppaml_tracer_t_size"
  ppaml_tracer_t_size :: Ptr CSize

tracerSize :: Int
tracerSize = fromIntegral $ unsafeReadLibraryConstant ppaml_tracer_t_size


----- Initialization and finalization -----

type TracerHandle = Ptr Tracer

foreign import ccall unsafe "ppaml/tracer.h ppaml_tracer_init"
  ppaml_tracer_init :: TracerHandle -> CString -> IO CInt

tracerInit :: TracerHandle -> FilePath -> IO ()
tracerInit tracer reportNameBase =
  withCString reportNameBase $ \cReportNameBase ->
    ppaml_tracer_init tracer cReportNameBase >>= \case
      0 -> return ()
      1 -> throw OTFManagerInitializationFailure
      2 -> throw OTFWriterInitializationFailure
      3 -> throw OTFTraceResolutionFailure
      4 -> throw OTFProcessDefinitionFailure
      r -> unexpectedReturnCode r

foreign import ccall unsafe "ppaml/tracer.h ppaml_tracer_init_from_env"
  ppaml_tracer_init_from_env :: TracerHandle -> IO CInt

tracerInitFromEnv :: TracerHandle -> IO ()
tracerInitFromEnv tracer =
  ppaml_tracer_init_from_env tracer >>= \case
    0 -> return ()
    1 -> throw OTFManagerInitializationFailure
    2 -> throw OTFWriterInitializationFailure
    3 -> throw OTFTraceResolutionFailure
    4 -> throw OTFProcessDefinitionFailure
    5 -> throw TraceBaseUndefined
    r -> unexpectedReturnCode r

foreign import ccall unsafe "ppaml/tracer.h ppaml_tracer_done"
  ppaml_tracer_done :: TracerHandle -> IO CInt

tracerDone :: TracerHandle -> IO ()
tracerDone tracer =
  ppaml_tracer_done tracer >>= \case 0 -> return ()
                                     1 -> throw OTFWriterCloseFailure
                                     r -> unexpectedReturnCode r


------------------------------------ Phases ------------------------------------

----- Data type -----

-- | A phase of execution to trace and to gather timing statistics about.
data Phase

foreign import ccall unsafe "ppaml/tracer/internal.h & ppaml_phase_t_size"
  ppaml_phase_t_size :: Ptr CSize

phaseSize :: Int
phaseSize = fromIntegral $ unsafeReadLibraryConstant ppaml_phase_t_size


----- Initialization and finalization -----

type PhaseHandle = Ptr Phase

foreign import ccall unsafe "ppaml/tracer.h ppaml_phase_init"
  ppaml_phase_init :: TracerHandle -> PhaseHandle -> CString -> IO CInt

phaseInit :: TracerHandle -> PhaseHandle -> String -> IO ()
phaseInit tracer phase name =
  withCString name $ \cName ->
    ppaml_phase_init tracer phase cName >>= \case
      0 -> return ()
      1 -> throw OTFPhaseDefinitionFailure
      r -> unexpectedReturnCode r

foreign import ccall unsafe "ppaml/tracer.h ppaml_phase_done"
  ppaml_phase_done :: PhaseHandle -> IO CInt

phaseDone :: PhaseHandle -> IO ()
phaseDone phase =
  ppaml_phase_done phase >>= \case 0 -> return ()
                                   r -> unexpectedReturnCode r


----- Timing -----

foreign import ccall unsafe "ppaml/tracer.h ppaml_phase_start"
  ppaml_phase_start :: PhaseHandle -> IO CInt

phaseStart :: PhaseHandle -> IO ()
phaseStart phase =
  ppaml_phase_start phase >>= \case 0 -> return ()
                                    1 -> throw ClockAcquisitionFailure
                                    2 -> throw OTFPhaseEntryFailure
                                    r -> unexpectedReturnCode r

foreign import ccall unsafe "ppaml/tracer.h ppaml_phase_stop"
  ppaml_phase_stop :: PhaseHandle -> IO CInt

phaseStop :: PhaseHandle -> IO ()
phaseStop phase =
  ppaml_phase_stop phase >>= \case 0 -> return ()
                                   1 -> throw ClockAcquisitionFailure
                                   2 -> throw OTFPhaseExitFailure
                                   r -> unexpectedReturnCode r


----------------------------------- Utility ------------------------------------

unsafeReadLibraryConstant :: Storable a => Ptr a -> a
unsafeReadLibraryConstant = unsafeDupablePerformIO . peek

unexpectedReturnCode :: CInt -> a
unexpectedReturnCode code =
  error $ "unexpected return code " ++ show code ++ "\n\
          \This is a bug in ppamltracer!  Report it to the maintainers."
