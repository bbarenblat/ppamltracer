{- PPAML.Tracer.Exception -- exceptions used by ppamltracer
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

{-# LANGUAGE DeriveDataTypeable #-}
{-# LANGUAGE ExistentialQuantification #-}
{-# LANGUAGE TemplateHaskell #-}
module PPAML.Tracer.Exception where

import Control.Exception (SomeException(SomeException))
import Control.Exception.Hierarchical (mkAbstractException, mkException)

-- | A generic ppamltracer error.  Extends 'SomeException'.
mkAbstractException 'SomeException "TracerException"


-------------------------------- OTF exceptions --------------------------------

{-| An error related to Open Trace Format input and output.  Extends
'TracerException'. -}
mkAbstractException 'TracerException "OTFException"

-- | An error caused by the Open Trace Format manager.  Extends 'OTFException'.
mkAbstractException 'OTFException "OTFManagerException"

{-| Failure to initialize the Open Trace Format manager.  Extends
'OTFManagerException'. -}
mkException 'OTFManagerException "OTFManagerInitializationFailure"

-- | An error caused by the Open Trace Format writer.  Extends 'OTFException'.
mkAbstractException 'OTFException "OTFWriterException"

{-| Failure to initialize the Open Trace Format writer.  Extends
'OTFWriterException'. -}
mkException 'OTFWriterException "OTFWriterInitializationFailure"

-- | Failure to define a phase.  Extends 'OTFWriterException'.
mkException 'OTFWriterException "OTFPhaseDefinitionFailure"

-- | Failure to record entry into a phase.  Extends 'OTFWriterException'.
mkException 'OTFWriterException "OTFPhaseEntryFailure"

-- | Failure to record exit from a phase.  Extends 'OTFWriterException'.
mkException 'OTFWriterException "OTFPhaseExitFailure"

{-| Failure to close the Open Trace Format writer.  Extends
'OTFWriterException'. -}
mkException 'OTFWriterException "OTFWriterCloseFailure"

-- | Failure to set the tracer resolution.  Extends 'OTFWriterException'.
mkException 'OTFWriterException "OTFTraceResolutionFailure"

-- | Failure to define a process (i.e., a thread).  Extends 'OTFWriterException'.
mkException 'OTFWriterException "OTFProcessDefinitionFailure"

{-| Thrown by 'withTracer'' when it is called and 'PPAMLTRACER_TRACE_BASE' is
unset or empty. -}
mkException 'TracerException "TraceBaseUndefined"


------------------------------ Timing exceptions -------------------------------

-- | An error related to system timers.  Extends 'TracerException'.
mkAbstractException 'TracerException "TimingException"

-- | A failure to get the current clock time.  Extends 'TimingException'.
mkException 'TimingException "ClockAcquisitionFailure"
