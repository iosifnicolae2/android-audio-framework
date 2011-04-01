#include "LameEncoder.h"


LameEncoder::LameEncoder(void)
{
}


LameEncoder::~LameEncoder(void)
{
}

bool LameEncoder::init(EncoderConfiguration* configuration) {
	bool result = false;
	if (!initialized) {
		lame = lame_init();
		lame_set_in_samplerate(lame, configuration->getInSamplerate());
		lame_set_num_channels(lame, configuration->getNumChannels());
		lame_set_quality(lame, configuration->getQuality());
		lame_set_mode(lame, (MPEG_mode) configuration->getMode());
		lame_set_brate(lame, configuration->getBitrate());
		if (lame_init_params(lame) != -1) {
			result = initialized = true;
			this->configuration = configuration;
			if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Lame has been initialized");
		} else {
			if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Something failed while initializing Lame");
		}
	}
	return result;
}

int LameEncoder::getBufferSize(int pcmBufferSize) {
	return (int) (1.25 * pcmBufferSize  + 7200);
}

int LameEncoder::encode(short int* pcm, int length, unsigned char * output) {
	return lame_encode_buffer(lame, pcm, pcm, length, output, getBufferSize(length));
}

unsigned char* LameEncoder::flush(int* resultSize) {	
	int enc_size = 7200 * 2;
	unsigned char* buffer = new unsigned char[enc_size];
	*resultSize = lame_encode_flush(lame, buffer, enc_size);
	if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "flush result = %D", *resultSize);
	return buffer;
}

void LameEncoder::close() {
	if (initialized) {
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Lame has been closed");
		lame_close(lame);
		lame = NULL;
		initialized = false;
	}
}