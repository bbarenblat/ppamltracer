/* check_ppamltracer.c -- test program for ppamltracer
 * Copyright (C) 2014  Galois, Inc.
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

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_TIME_H
#	include <time.h>
#endif
#if HAVE_UNISTD_H
#	include <unistd.h>
#endif

#include <check.h>

#include <otf.h>
#include <ppaml/tracer.h>

#include "tmpdir.h"
#include "trace_invalid.h"
#include "trace_valid.h"

#define ignore(x) ((void)(x))

#if (HAVE_FIXTURE_tmpdir && HAVE_FIXTURE_trace_invalid && HAVE_UNISTD_H)
#	define HAVE_TEST_init_creates_trace
	START_TEST(test_init_creates_trace)
	{
		char index_file[128];
		strcpy(index_file, trace_invalid_path());
		strcat(index_file, ".otf");
		ck_assert(access(index_file, F_OK) == 0);
	}
	END_TEST
#endif

#if (HAVE_FIXTURE_tmpdir && HAVE_UNISTD_H && HAVE_SETENV)
#	define HAVE_TEST_init_from_env
	START_TEST(test_init_from_env)
	{
		// Figure out where to put the trace.
		char trace_base[128];
		strcpy(trace_base, tmpdir_name());
		strcat(trace_base, "/trace");
		// Tell ppamltracer where.
		ck_assert(
			setenv("PPAMLTRACER_TRACE_BASE", trace_base, 1) == 0);
		// Create the trace.
		ppaml_tracer_t tracer;
		ck_assert(
			ppaml_tracer_init_from_env(&tracer) == 0);
		ck_assert(ppaml_tracer_done(&tracer) == 0);
		// Make sure it's in the right place.
		strcat(trace_base, ".otf");
		ck_assert(access(trace_base, F_OK) == 0);
	}
	END_TEST

#	define HAVE_TEST_init_from_env_fails_on_empty
	START_TEST(test_init_from_env_fails_on_empty)
	{
		ck_assert(setenv("PPAMLTRACER_TRACE_BASE", "", 1) == 0);
		ppaml_tracer_t tracer;
		ck_assert(ppaml_tracer_init_from_env(&tracer) == 5);
	}
	END_TEST
#endif

#if (HAVE_FIXTURE_tmpdir && HAVE_UNISTD_H && HAVE_UNSETENV)
#	define HAVE_TEST_init_from_env_fails_on_unset
	START_TEST(test_init_from_env_fails_on_unset)
	{
		ck_assert(unsetenv("PPAMLTRACER_TRACE_BASE") == 0);
		ppaml_tracer_t tracer;
		ck_assert(ppaml_tracer_init_from_env(&tracer) == 5);
	}
	END_TEST
#endif

#if (HAVE_FIXTURE_tmpdir && HAVE_FIXTURE_trace_valid)
#	define HAVE_TEST_simple_creates_valid_trace
	START_TEST(test_simple_creates_valid_trace)
	{
		OTF_FileManager *const manager = OTF_FileManager_open(16);
		ck_assert_msg(manager != NULL, otf_strerr);
		OTF_Reader *const reader =
			OTF_Reader_open(trace_valid_path(), manager);
		ck_assert_msg(reader != NULL, otf_strerr);
		ck_assert_msg(OTF_Reader_close(reader) == 1, otf_strerr);
		OTF_FileManager_close(manager);
	}
	END_TEST

#	define HAVE_TEST_simple_defines_main
	START_TEST(test_simple_defines_main)
	{
		OTF_FileManager *const manager = OTF_FileManager_open(16);
		ck_assert_msg(manager != NULL, otf_strerr);
		OTF_Reader *const reader =
			OTF_Reader_open(trace_valid_path(), manager);
		ck_assert_msg(reader != NULL, otf_strerr);
		ck_assert_msg(OTF_Reader_close(reader) == 1, otf_strerr);
		OTF_FileManager_close(manager);
	}
	END_TEST
#endif

#if (HAVE_FIXTURE_tmpdir && HAVE_TIME_H)
#	define HAVE_TEST_trace_accurate_timing
	static uint64_t test_trace_accurate_timing_delta;

	static int test_trace_accurate_timing_handle_enter(
		void *const userData,
		const uint64_t time,
		const uint32_t function,
		const uint32_t process,
		const uint32_t source)
	{
		ignore(userData);
		ignore(function);
		ignore(process);
		ignore(source);
		test_trace_accurate_timing_delta = time;
		return OTF_RETURN_OK;
	}

	static int test_trace_accurate_timing_handle_exit(
		void *const userData,
		const uint64_t time,
		const uint32_t function,
		const uint32_t process,
		const uint32_t source)
	{
		ignore(userData);
		ignore(function);
		ignore(process);
		ignore(source);
		test_trace_accurate_timing_delta =
			time - test_trace_accurate_timing_delta;
		return OTF_RETURN_OK;
	}

	START_TEST(test_trace_accurate_timing)
	{
		// Create the trace.
		char trace_base[128];
		strcpy(trace_base, tmpdir_name());
		strcat(trace_base, "/trace");
		ppaml_tracer_t tracer;
		ck_assert(
			ppaml_tracer_init(&tracer, trace_base) == 0);
		ppaml_phase_t phase;
		ck_assert(
			ppaml_phase_init(&tracer, &phase, "test phase") == 0);
		// Time a single phase.
		ck_assert(ppaml_phase_start(&phase) == 0);
		const struct timespec hundred_ms =
			{ .tv_sec = 0, .tv_nsec = 100000000 };
		nanosleep(&hundred_ms, NULL);
		ck_assert(ppaml_phase_stop(&phase) == 0);
		// Clean up and close the trace.
		ck_assert(ppaml_phase_done(&phase) == 0);
		ck_assert(ppaml_tracer_done(&tracer) == 0);
		/* Verify that the delta is about 100 ms (100,000 ticks of the
		 * tracer timer). */
		OTF_FileManager *const manager = OTF_FileManager_open(16);
		ck_assert_msg(manager != NULL, otf_strerr);
		OTF_Reader *const reader =
			OTF_Reader_open(trace_base, manager);
		ck_assert_msg(reader != NULL, otf_strerr);
		OTF_HandlerArray *const handlers = OTF_HandlerArray_open();
		ck_assert_msg(handlers != NULL, otf_strerr);
		ck_assert_msg(
			OTF_HandlerArray_setHandler(
				handlers,
				(OTF_FunctionPointer *)test_trace_accurate_timing_handle_enter,
				OTF_ENTER_RECORD)
			== 1,
			otf_strerr);
		ck_assert_msg(
			OTF_HandlerArray_setHandler(
				handlers,
				(OTF_FunctionPointer *)test_trace_accurate_timing_handle_exit,
				OTF_LEAVE_RECORD)
			== 1,
			otf_strerr);
		ck_assert_msg(
			OTF_Reader_readEvents(reader, handlers) != OTF_READ_ERROR,
			otf_strerr);
		/* Require less than +/- 10% to pass.  Yes, this is an
		 * arbitrary metric, but it should trigger if there's a serious
		 * regression. */
		ck_assert_msg(
			90000 <= test_trace_accurate_timing_delta,
			"timed interval (%d us) is more than 10%% fast",
			test_trace_accurate_timing_delta);
		ck_assert_msg(test_trace_accurate_timing_delta < 110000,
			"timed interval (%d us) is more than 10%% slow",
			test_trace_accurate_timing_delta);
		// All done.
		ck_assert_msg(
			OTF_HandlerArray_close(handlers) == 1, otf_strerr);
		ck_assert_msg(OTF_Reader_close(reader) == 1, otf_strerr);
		OTF_FileManager_close(manager);
	}
	END_TEST
#endif

Suite *ppamltracer_suite()
{
	Suite *const s = suite_create("ppamltracer");
	// Test case: init
	TCase *tc = tcase_create("init");
	tmpdir_add_checked(tc);
	trace_invalid_add_checked(tc);
#	ifdef HAVE_TEST_init_creates_trace
		tcase_add_test(tc, test_init_creates_trace);
#	endif
	suite_add_tcase(s, tc);
	// Test case: init from env
	tc = tcase_create("init_from_env");
	tmpdir_add_checked(tc);
#	ifdef HAVE_TEST_init_from_env
		tcase_add_test(tc, test_init_from_env);
#	endif
#	ifdef HAVE_TEST_init_from_env_fails_on_unset
		tcase_add_test(tc, test_init_from_env_fails_on_unset);
#	endif
#	ifdef HAVE_TEST_init_from_env_fails_on_empty
		tcase_add_test(tc, test_init_from_env_fails_on_empty);
#	endif
	suite_add_tcase(s, tc);
	// Test case: valid
	tc = tcase_create("valid");
	tmpdir_add_checked(tc);
	trace_valid_add_checked(tc);
#	ifdef HAVE_TEST_simple_creates_valid_trace
		tcase_add_test(tc, test_simple_creates_valid_trace);
#	endif
#	ifdef HAVE_TEST_simple_defines_main
		tcase_add_test(tc, test_simple_defines_main);
#	endif
	suite_add_tcase(s, tc);
	// Test case: timing-accurate
	tc = tcase_create("accurate");
	tmpdir_add_checked(tc);
#	ifdef HAVE_TEST_trace_accurate_timing
		tcase_add_test(tc, test_trace_accurate_timing);
#	endif
	suite_add_tcase(s, tc);
	return s;
}

int main()
{
	Suite *const s = ppamltracer_suite();
	SRunner *const runner = srunner_create(s);
	srunner_run_all(runner, CK_NORMAL);
	const int n_failed = srunner_ntests_failed(runner);
	srunner_free(runner);
	return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
