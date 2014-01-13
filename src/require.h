/* require.h -- useful error-handling macros
 * Copyright (C) 2013  Galois, Inc.
 *
 * This library is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * To contact Galois, complete the Web form at
 * <http://corp.galois.com/contact/> or write to Galois, Inc., 421 Southwest
 * 6th Avenue, Suite 300, Portland, Oregon, 97204-1622. */

#ifndef REQUIRE_H
#define REQUIRE_H

#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#define require_not(var, val, ret_val)		\
	do {                                    \
		if ((var) != val) {		\
			r = ret_val;            \
			goto done;              \
		}                               \
	} while (0)
#define require_zero(var, ret_val) require_not(var, 0, ret_val)
#define require_one(var, ret_val) require_not(var, 1, ret_val)

#define require_nonnull(ptr, ret_val)           \
	do {                                    \
		if ((ptr) == NULL) {            \
			r = ret_val;            \
			goto done;              \
		}                               \
	} while (0)

#endif
