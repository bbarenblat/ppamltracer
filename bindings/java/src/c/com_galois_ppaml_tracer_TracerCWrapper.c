/* com_galois_ppaml_tracer_TracerCWrapper.c -- JNI wrapper for
 *     com.galois.ppaml.tracer.TracerCWrapper
 * Copyright (C) 2014  Galois, Inc.
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

#include <stdlib.h>

#include <ppaml/tracer.h>

#include <jni.h>
#include <com_galois_ppaml_tracer_TracerCWrapper.h>

#include "pointer.h"

JNIEXPORT
jlong JNICALL Java_com_galois_ppaml_tracer_TracerCWrapper_mallocTracer(
	JNIEnv *const env,
	jclass Tracer)
{
	return jlong_of_pointer(malloc(sizeof(ppaml_tracer_t)));
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_TracerCWrapper_ppaml_1tracer_1init(
	JNIEnv *const env,
	jclass Tracer,
	jlong tracer,
	jstring reportNameBase)
{
	jint r;
	// Allocate memory.
	const char *const cReportNameBase =
		(*env)->GetStringUTFChars(env, reportNameBase, NULL);
	if (cReportNameBase == NULL) {
		r = -1;
		goto done;
	}
	// Perform the operation.
	r = ppaml_tracer_init(pointer_of_jlong(tracer), cReportNameBase);
	// Clean up.
done:	(*env)->ReleaseStringUTFChars(env, reportNameBase, cReportNameBase);
	return r;
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_TracerCWrapper_ppaml_1tracer_1init_1from_1env(
	JNIEnv *const env,
	jclass Tracer,
	jlong tracer)
{
	return ppaml_tracer_init_from_env(pointer_of_jlong(tracer));
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_TracerCWrapper_ppaml_1tracer_1done(
	JNIEnv *const env,
	jclass Tracer,
	jlong tracer)
{
	return ppaml_tracer_done(pointer_of_jlong(tracer));
}

JNIEXPORT
void JNICALL Java_com_galois_ppaml_tracer_TracerCWrapper_freeTracer(
	JNIEnv *const env,
	jclass Tracer,
	jlong tracer)
{
	free(pointer_of_jlong(tracer));
}
