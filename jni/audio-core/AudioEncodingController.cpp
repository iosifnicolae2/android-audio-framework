#include "AudioEncodingController.h"


AudioEncodingController::AudioEncodingController(void)
	: encoderType(-1), encoder(NULL)
{
}


AudioEncodingController::~AudioEncodingController(void)
{
	close();
}

bool AudioEncodingController::init(int encoderType, EncoderConfiguration* configuration) {
	bool result = false;
	if (!encoder) {
		switch (encoderType) {
		case FAAC_ENCODER: encoder = new AacEncoder(); break;
		case LAME_ENCODER: encoder = new LameEncoder(); break;
		case OGG_ENCODER: encoder = NULL; break;
		}
		if (encoder) {
			result = encoder->init(configuration);
			if (!result) {
				close();
			}
		}
	}
	return result;
}

int AudioEncodingController::getEncoderType(){
	return encoderType;
}

AudioEncoder* AudioEncodingController::getEncoder() {
	return encoder;
}

void AudioEncodingController::close(){
	if (encoder) {
		encoder->close();
		delete encoder;
		encoder = NULL;
		encoderType = -1;
	}
}