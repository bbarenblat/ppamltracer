/* com_galois_ppaml_tracer_TracerCWrapper.c -- JNI wrapper for
 *     com.galois.ppaml.tracer.TracerCWrapper
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
