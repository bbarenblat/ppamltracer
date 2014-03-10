/* com_galois_ppaml_tracer_Phase.c -- JNI wrapper for com.galois.ppaml.tracer.Phase
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
