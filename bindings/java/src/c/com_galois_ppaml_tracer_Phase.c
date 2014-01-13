/* com_galois_ppaml_tracer_Phase.c -- JNI wrapper for com.galois.ppaml.tracer.Phase
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
#include <com_galois_ppaml_tracer_Phase.h>

#include "pointer.h"

JNIEXPORT
jlong JNICALL Java_com_galois_ppaml_tracer_Phase_mallocPhase(
	JNIEnv *const env,
	jclass Phase)
{
	return jlong_of_pointer(malloc(sizeof(ppaml_phase_t)));
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_Phase_ppaml_1phase_1init(
	JNIEnv *const env,
	jclass Phase,
	jlong tracer,
	jlong phase,
	jstring name)
{
	jint r;
	// Allocate memory.
	const char *const cName = (*env)->GetStringUTFChars(env, name, NULL);
	if (cName == NULL) {
		r = -1;
		goto done;
	}
	// Perform the operation.
	r = ppaml_phase_init(
		pointer_of_jlong(tracer),
		pointer_of_jlong(phase),
		cName);
	// Clean up.
done:	(*env)->ReleaseStringUTFChars(env, name, cName);
	return r;
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_Phase_ppaml_1phase_1done(
	JNIEnv *const env,
	jclass Phase,
	jlong phase)
{
	return ppaml_phase_done(pointer_of_jlong(phase));
}

JNIEXPORT
void JNICALL Java_com_galois_ppaml_tracer_Phase_freePhase(
	JNIEnv *const env,
	jclass Phase,
	jlong phase)
{
	free(pointer_of_jlong(phase));
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_Phase_ppaml_1phase_1start(
	JNIEnv *const env,
	jclass Phase,
	jlong phase)
{
	return ppaml_phase_start(pointer_of_jlong(phase));
}

JNIEXPORT
jint JNICALL Java_com_galois_ppaml_tracer_Phase_ppaml_1phase_1stop(
	JNIEnv *const env,
	jclass Phase,
	jlong phase)
{
	return ppaml_phase_stop(pointer_of_jlong(phase));
}
