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

AudioEncodingController* controller;
unsigned char * encodingBuffer = NULL;
int encodingBufferSize = 0;

JNIEXPORT
jboolean initialize(JNIEnv* env, jobject thiz, jintArray configuration) {
	EncoderConfiguration *encoderConfiguration = new EncoderConfiguration();
	int inConfig[configLen];

	env->GetIntArrayRegion(configuration, 0, configLen, (jint*)inConfig);
	encoderConfiguration->setInSamplerate(inConfig[inSamplerate]);
	encoderConfiguration->setNumChannels(inConfig[numChannels]);
	encoderConfiguration->setQuality(inConfig[quality]);
	encoderConfiguration->setMode(inConfig[mode]);
	encoderConfiguration->setBitrate(inConfig[bitrate]);
	encoderConfiguration->setBitPerSample(inConfig[bitPerSample]);

	if (!controller) {
		controller = new AudioEncodingController();
	}
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
		controller->close();
		delete controller;
		if (encodingBuffer) delete[] encodingBuffer;
		encodingBufferSize = 0;
	}
}

int methodsCount = 5;
JNINativeMethod methods[] = {
	{"initialize",	"([I)Z", (bool*)initialize },
	{"getOutputBufferSize",	"()I",	(int*)getOutputBufferSize },
	{"encode", "([BII[B)I", (void*)encode},
	{"flush", "()[B", (void*)flush},
	{"close", "()V", (void*)close}
};
const char *classPathName = "com/shaubert/android/aaf/jni/AudioCoreJNI";

int jniRegisterNativeMethods(JNIEnv* env, const char* className,
    const JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"Registering %s natives", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"Native registration unable to find class '%s'", className);
        return -1;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"RegisterNatives failed for '%s'", className);
        return -1;
    }
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"ERROR: GetEnv failed");
        goto bail;
    }

	if (jniRegisterNativeMethods(env, classPathName, methods, methodsCount) < 0) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"ERROR: native registration failed");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}