#include <jni.h>
#include "AudioEncodingController.h"

const int encoderType = 0;
const int inSamplerate = 1;
const int numChannels = 2;
const int quality = 3;
const int mode = 4;
const int bitrate = 5;
const int bitPerSample = 6;

const int configLen = 7;

AudioEncodingController* controller = NULL;
unsigned char * encodingBuffer = NULL;
int encodingBufferSize = 0;

bool logging = true;

JNIEXPORT
jboolean initialize(JNIEnv* env, jobject thiz, jintArray configuration) {
	EncoderConfiguration *encoderConfiguration = new EncoderConfiguration();
	int inConfig[configLen];

	if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "reading configuration");
	env->GetIntArrayRegion(configuration, 0, configLen, (jint*)&inConfig);
	encoderConfiguration->setInSamplerate(inConfig[inSamplerate]);
	encoderConfiguration->setNumChannels(inConfig[numChannels]);
	encoderConfiguration->setQuality(inConfig[quality]);
	encoderConfiguration->setMode(inConfig[mode]);
	encoderConfiguration->setBitrate(inConfig[bitrate]);
	encoderConfiguration->setBitPerSample(inConfig[bitPerSample]);

	if (!controller) {
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "creating new controller...");
		controller = new AudioEncodingController();
	}
	if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "initializing controller...");
	return controller->init(inConfig[encoderType], encoderConfiguration);
}

JNIEXPORT
jint getOutputBufferSize(JNIEnv* env, jobject thiz, jint pcmBufferSizeInSamples) {
	return controller->getEncoder()->getBufferSize(pcmBufferSizeInSamples);
}

void
increaseBufferSizeIfNeeded(int inputSamplesCount) {
	int newSize = controller->getEncoder()->getBufferSize(inputSamplesCount);
	if (encodingBufferSize < newSize) {
		if (encodingBuffer) delete[] encodingBuffer;
		encodingBuffer = new unsigned char[newSize];
		encodingBufferSize = newSize;
	}
}

JNIEXPORT
jint encode(JNIEnv* env, jobject thiz, jbyteArray inputPcm, jint offset, jint length, jbyteArray output) {
	int samplesInput = length / 2;
	short int* input = new short int[samplesInput];
	env->GetByteArrayRegion(inputPcm, offset, length, (jbyte*)input);
	increaseBufferSizeIfNeeded(samplesInput);
	int result = controller->getEncoder()->encode(input, samplesInput, encodingBuffer);
	if (result) {
		env->SetByteArrayRegion(output, 0, result, (jbyte*)encodingBuffer);
	}
	delete[] input;
	return result;
}

JNIEXPORT
jbyteArray flush(JNIEnv* env, jobject thiz) {
	jbyteArray output = NULL;
	int outputSize = -1;
	unsigned char* result;
	result = controller->getEncoder()->flush(&outputSize);
	if (result) {
		if (outputSize) {
			output = env->NewByteArray(outputSize);
			if (output) {
				env->SetByteArrayRegion(output, 0, outputSize, (jbyte*)encodingBuffer);
			}
		} else {
			delete[] result;
		}
	}
	return output;
}

JNIEXPORT
void close(JNIEnv* env, jobject thiz) {
	if (controller) {
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "closing controller...");
		controller->close();
		delete controller;
		controller = NULL;
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "controller has been closed");
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "clearing buffers...");
		if (encodingBuffer) delete[] encodingBuffer;
		encodingBuffer = NULL;
		encodingBufferSize = 0;
	}
	if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "close() finished");
}

JNIEXPORT
void setLogging(JNIEnv* env, jobject thiz, jboolean on) {
	logging = on;
	if (controller) {
		controller->getEncoder()->setLogging(on);
	}
}


int methodsCount = 6;
JNINativeMethod methods[] = {
	{"nativeInitialize", "([I)Z", (bool*)initialize },
	{"nativeGetOutputBufferSize", "(I)I",	(int*)getOutputBufferSize },
	{"nativeEncode", "([BII[B)I", (void*)encode},
	{"nativeFlush", "()[B", (void*)flush},
	{"nativeClose", "()V", (void*)close},
	{"nativeSetLogging", "(Z)V", (void*)setLogging}
};
const char *classPathName = "com/shaubert/android/aaf/jni/AudioCoreJNI";

int jniRegisterNativeMethods(JNIEnv* env, const char* className,
    const JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"Registering %s natives", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
    	if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"Native registration unable to find class '%s'", className);
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
    	if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"RegisterNatives failed for '%s'", className);
        return -1;
    }
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
    	if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"ERROR: GetEnv failed");
        goto bail;
    }

	if (jniRegisterNativeMethods(env, classPathName, methods, methodsCount) < 0) {
        if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"ERROR: native registration failed");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}
