/* check_ppamltracer.c -- test program for ppamltracer
 * Copyright (C) 2014  Galois, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at
 * <http://corp.galois.com/contact/> or write to Galois, Inc., 421 Southwest
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#define _POSIX_C_SOURCE 199309L

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