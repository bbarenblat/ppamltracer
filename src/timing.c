/* timing.c -- cross-platform wall time
 * Copyright (C) 2013, 2014  Galois, Inc.
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

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif
#define _POSIX_C_SOURCE 199309L

#include <stdint.h>

#include "timing.h"
#include "require.h"

#define ignore(var) ((void)(var))

const uint64_t MICRO_PER_BASE = 1000000;
const uint64_t NANO_PER_MICRO = 1000;

// Try to use the POSIX timers, if possible.
#if HAVE_UNISTD_H
#	include <unistd.h>
#endif
#if (HAVE_UNISTD_H && _POSIX_TIMERS > 0)

#	include <time.h>

#	if __linux__
		/* Hopefully, you're running linux >=2.6.28, and
		 * CLOCK_MONOTONIC_RAW exists. */
		static const clockid_t CLOCK_ID = CLOCK_MONOTONIC_RAW;
#	elif _POSIX_MONOTONIC_CLOCK
		// You're not on Linux, but you've got a POSIX monotonic clock.
		static const clockid_t CLOCK_ID = CLOCK_MONOTONIC;
#	else
		// Just use the POSIX wall clock.
		static const clockid_t CLOCK_ID = CLOCK_REALTIME;
#	endif

	int timing_get_wall(uint64_t *const result)
	{
		int r = 0;
		struct timespec now;
		require_zero(clock_gettime(CLOCK_ID, &now), 1);
		*result = (uint64_t)now.tv_sec * MICRO_PER_BASE
			+ (uint64_t)now.tv_nsec / NANO_PER_MICRO;
	done:	return r;
	}

#elif (__APPLE__ && __MACH__)

	/* No POSIX timers, but we're on a Mac, so we can use
	 * 'mach_absolute_time'. */
#	include <mach/mach_time.h>

	int timing_get_wall(uint64_t *const result)
	{
		static mach_timebase_info_data_t timebase_info;
		int r = 0;
		*result = mach_absolute_time();
		/* *result is now in units of Mach ticks.  Convert that to
                 * nanoseconds using the timebase information that Mach
                 * provides.  Note that 'timebase_info' is static, so it got
                 * zeroed out at the start of the program run; if its
                 * denominator is still zero, we've definitely not stored
                 * anything in it yet. */
		if (timebase_info.denom == 0) {
			require_zero(mach_timebase_info(&timebase_info), 1);
		}
		*result = *result * timebase_info.numer / timebase_info.denom;
		// Convert nanoseconds to microseconds.
		*result /= NANO_PER_MICRO;
	done:	return r;
	}

#else

	// I have no clue what wacky platform you're on.
#	error Unsupported timing platform

	int timing_get_wall(uint64_t *const result)
	{
		ignore(result);
		return -1;
	}

#endif
