/* simple.c -- basic ppamltracer example
 * This file is in the public domain.
 *
 * Compile this with
 *     cc -std=c99 -lppamltracer -o simple simple.c
 */

#include <stdio.h>
#include <stddef.h>

#include <ppaml/tracer.h>

// ppamltracer state
ppaml_tracer_t tracer;
ppaml_phase_t phase_fact;
ppaml_phase_t phase_fib;


// Prototypes
unsigned long long fact(const unsigned short);
unsigned long fib(const unsigned short);


int main(int argc, char *argv[])
{
	/* Disable buffering on stdout so we can see the numbers as they are
	 * printed. */
	setvbuf(stdout, NULL, _IONBF, 0);

	// Start ppamltracer.
	ppaml_tracer_init(&tracer, "/tmp/simple_report");

	// Register the phases.
	ppaml_phase_init(&tracer, &phase_fact, "fact");
	ppaml_phase_init(&tracer, &phase_fib, "fib");

	// Print factorials.
	printf("Factorials: ");
	for (unsigned short i = 0; i < 21; i++) {
		printf("%lld ", fact(i));
	}
	printf("\n");

	// Print Fibonacci numbers.
	printf("Fibonacci numbers: ");
	for (unsigned short i = 0; i < 28; i++) {
		printf("%ld ", fib(i));
	}
	printf("\n");

	// Finalize ppamltracer and exit.
	ppaml_phase_done(&phase_fact);
	ppaml_phase_done(&phase_fib);
	ppaml_tracer_done(&tracer);
	return 0;
}

unsigned long long fact(const unsigned short n)
{
	// Record that we have entered the 'fact' phase.
	ppaml_phase_start(&phase_fact);

	// Compute the factorial.
	unsigned long long result;
	if (n == 0) {
		result = 1;
	} else {
		result = (unsigned long long)n * fact(n - 1);
	}

	// Record that we're exiting the 'fact' phase.
	ppaml_phase_stop(&phase_fact);
	return result;
}

unsigned long fib(const unsigned short n)
{
	// Record that we have entered the 'fib' phase.
	ppaml_phase_start(&phase_fib);

	// Compute the nth Fibonancci number.
	unsigned long result;
	if (n == 0 || n == 1) {
		result = 1;
	} else {
		result = fib(n - 1) + fib(n - 2);
	}

	// Record that we're exiting the 'fact' phase.
	ppaml_phase_stop(&phase_fib);
	return result;
}
