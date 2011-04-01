#pragma once

#include "EncoderConfiguration.h"
#include <android/log.h>
#include <stdlib.h>
#include <stdint.h>

static const char* LOG_TAG = "JNI_AUDIO_ENCODING";

class AudioEncoder
{

protected:
	EncoderConfiguration* configuration;
	bool initialized;
	bool logging;

public:

	AudioEncoder(void)
	{
	}

	virtual ~AudioEncoder(void)
	{
	}

	bool isLogging() {
		return logging;
	}

	void setLogging(bool on) {
		logging = on;
	}

	virtual bool init(EncoderConfiguration* configuration);
	virtual int getBufferSize(int pcmBufferSize);
	virtual int encode(short int* pcm, int length, unsigned char * output);
	virtual unsigned char* flush(int* resultSize);
	virtual void close();


};

