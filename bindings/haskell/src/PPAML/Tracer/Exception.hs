{- PPAML.Tracer.Exception -- exceptions used by ppamltracer
Copyright (C) 2014  Galois, Inc.

This library is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this library.  If not, see <http://www.gnu.org/licenses/>.

To contact Galois, complete the Web form at <http://corp.galois.com/contact/> or
write to Galois, Inc., 421 Southwest 6th Avenue, Suite 300, Portland, Oregon,
97204-1622. -}

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

-- | An error caused by the Open Trace Fromat manager.  Extends 'OTFException'.
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


------------------------------ Timing exceptions -------------------------------

-- | An error related to system timers.  Extends 'TracerException'.
mkAbstractException 'TracerException "TimingException"

-- | A failure to get the current clock time.  Extends 'TimingException'.
mkException 'TimingException "ClockAcquisitionFailure"
