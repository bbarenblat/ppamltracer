/* Simple -- basic ppamltracer example
 * This file is in the public domain. */

package com.galois.ppaml.tracer.examples;

import com.galois.ppaml.tracer.ExplicitlyConfiguredTracer;
import com.galois.ppaml.tracer.Phase;
import com.galois.ppaml.tracer.Tracer;

public class Simple {

	protected Phase factPhase;

	protected Phase fibPhase;

	protected void go(Tracer tracer) {
		// Register the phases.
		factPhase = tracer.createPhase("fact");
		fibPhase = tracer.createPhase("fib");

		// Print factorials.
		System.out.print("Factorials:");
		for (int i = 0; i < 21; i++) {
			System.out.print(" " + fact(i));
		}
		System.out.println();

		// Print Fibonacci numbers.
		System.out.print("Fibonacci numbers:");
		for (int i = 0; i < 30; i++) {
			System.out.print(" " + fib(i));
		}
		System.out.println();

		// Finalize phases and exit.
		factPhase.close();
		fibPhase.close();
	}

	protected long fact(int n) {
		// Record that we're running inside the factorial phase.
		factPhase.start();
		// Compute the factorial.
		long result;
		if (n == 0) {
			result = 1;
		} else {
			result = n * fact(n - 1);
		}
		// Return the result.
		factPhase.stop();
		return result;
	}

	protected int fib(int n) {
		// Record that we're running inside the Fibonacci phase.
		fibPhase.start();
		// Compute the nth Fibonacci number.
		int result;
		switch (n) {
		case 0:
		case 1:
			result = n;
			break;
		default:
			result = fib(n - 1) + fib(n - 2);
			break;
		}
		// Return the result.
		fibPhase.stop();
		return result;
	}

	public static void main(String[] args) {
		// Start ppamltracer.
		Tracer tracer = new ExplicitlyConfiguredTracer("/tmp/simple_report");

		// Run the app.
		(new Simple()).go(tracer);

		// Finalize ppamltracer and exit.
		tracer.close();
	}

}
