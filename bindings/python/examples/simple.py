#!/usr/bin/env python
# simple.py -- basic ppamltracer-python example
# This file is in the public domain.

import os
import sys

from ppamltracer import Tracer

def main():
    # Disable buffering on stdout so we can see the numbers as they are printed.
    sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
    # Start ppamltracer.
    with Tracer("/tmp/simple_report") as tracer:
        # Register the factorial phase.
        with tracer.create_phase("fact") as phase:
            # Print factorials.
            print "Factorials:",
            for i in range(21):
                print fact(phase, i),
            print
        # Register the Fibonacci phase.
        with tracer.create_phase("fib") as phase:
            # Print Fibonacci numbers.
            print "Fibonacci numbers: ",
            for i in range(24):
                print fib(phase, i),
            print

def fact(phase, n):
    # Record that we're running inside the factorial phase.
    with phase.running():
        # Compute the factorial.
        if n == 0:
            return 1
        else:
            return n * fact(phase, n - 1)

def fib(phase, n):
    # Record that we're running inside the Fibonacci phase.
    with phase.running():
        # Compute the nth Fibonacci number.
        if n == 0 or n == 1:
            return n
        else:
            return fib(phase, n - 1) + fib(phase, n - 2)

if __name__ == '__main__':
    main()
