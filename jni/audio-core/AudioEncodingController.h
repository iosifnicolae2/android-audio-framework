#pragma once

#include "AacEncoder.h"
#include "LameEncoder.h"

static const int FAAC_ENCODER = 1;
static const int LAME_ENCODER = 2;
static const int OGG_ENCODER = 3;

class AudioEncodingController
{
private:
	int encoderType;
	AudioEncoder* encoder;

public:
	AudioEncodingController(void);
	~AudioEncodingController(void);

	bool init(int encoderType, EncoderConfiguration* configuration);
	int getEncoderType();
	AudioEncoder* getEncoder();
	void close();
};