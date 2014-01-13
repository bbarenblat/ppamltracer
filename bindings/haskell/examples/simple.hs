{- simple.hs -- basic ppamltracer example
This file is in the public domain.

Compile this with
    ghc --make simple.hs
-}

{-# LANGUAGE LambdaCase #-}
module Main where

import Control.Applicative ((<$>), (<*>))
import Control.Monad (liftM)

import PPAML.Tracer

main :: IO ()
main = do
  -- Start ppamltracer.
  withTracer "/tmp/simple_report" $ \tracer -> do
    -- Register the factorial phase.
    withPhase tracer "fact" $ \phase -> do
      -- Print factorials.
      putStr "Factorials:"
      mapM_ (putStr . (' ':) . show) =<< mapM (fact phase) [0 .. 40]
      putStrLn ""
    -- Register the Fibonacci phase.
    withPhase tracer "fib" $ \phase -> do
      -- Print Fibonacci numbers.
      putStr "Fibonacci numbers:"
      mapM_ (putStr . (' ':) . show) =<< mapM (fib phase) [0 .. 25]
      putStrLn ""

{- Records that we're running inside the provided phase and computes a
factorial. -}
fact :: PhaseHandle -> Integer -> IO Integer
fact phase = withPhaseRunning phase . \case
               0 -> return 1
               n -> liftM (n*) $ fact phase (n - 1)

{- Records that we're running inside the provided phase and computes a Fibonacci
number. -}
fib :: PhaseHandle -> Integer -> IO Integer
fib phase = withPhaseRunning phase . \case
              0 -> return 0
              1 -> return 1
              n -> (+) <$> fib phase (n - 1) <*> fib phase (n - 2)
