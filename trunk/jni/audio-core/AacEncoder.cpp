#include "AacEncoder.h"

AacEncoder::AacEncoder(void)
{
}


AacEncoder::~AacEncoder(void)
{
	close();
}

bool AacEncoder::init(EncoderConfiguration* configuration)
{
	bool result = false;
	if (!initialized) {
		if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "initializing FAAC...");

		faac = faacEncOpen(configuration->getInSamplerate(), 
			configuration->getNumChannels(), &samplesInput, &maxBytesOutput);


		if (faac) {
			if (logging) {
				__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"FAAC Handler created");
				__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"max bytes output = %d", maxBytesOutput);
				__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"samplesInput = %d", samplesInput);
			}

			faacEncConfigurationPtr faacConf = faacEncGetCurrentConfiguration(faac);
			if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"configuring FAAC...");
			faacConf->aacObjectType = LOW;

			faacConf->quantqual = configuration->getQuality();
			faacConf->bitRate = configuration->getBitrate() * 1000;
			faacConf->outputFormat = 1; 
			faacConf->inputFormat = FAAC_INPUT_16BIT;
			int setConfigurationResult = faacEncSetConfiguration(faac, faacConf);

			if (setConfigurationResult == 1) {

				faac_flush_buffer_size = 0;
				faac_flush_buffer = new short[samplesInput];
				pcmBuf = new short[samplesInput];

				initialized = true;
				this->configuration = configuration;
				result = true;

				if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"FAAC has been initialized");
			} else {
				if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"FAAC has not been initialized");
			}
		} else {
			if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"FAAC Handler = NULL");
		}
	} 
	return result;
}

int AacEncoder::getBufferSize(int pcmBufferSize) {
	if (initialized) {
		return ((pcmBufferSize / samplesInput) + 1) * maxBytesOutput;
	} else {
		return -1;
	}
}

int AacEncoder::encode(short int* pcm, int length, unsigned char * output) {	
	memcpy(pcmBuf, faac_flush_buffer, sizeof(short) * faac_flush_buffer_size);
	int pcm_buf_pos = faac_flush_buffer_size;

	int enc_size = maxBytesOutput;
	int result = 0;
	int pcm_pos = 0;
	do {
		memcpy(pcmBuf + pcm_buf_pos, pcm + pcm_pos, sizeof(short) * (samplesInput - pcm_buf_pos));
		pcm_pos += samplesInput - pcm_buf_pos;
		
		int enc_result = faacEncEncode(faac, (int32_t *) pcmBuf, samplesInput, output + result, enc_size);
		if (enc_result >= 0) {
			result += enc_result;
		} else {
			result = enc_result;
		}

		pcm_buf_pos = 0;
	} while (result >= 0 && (length - pcm_pos >= samplesInput));

	if (result > 0) {
		faac_flush_buffer_size = length - pcm_pos;
		if (faac_flush_buffer_size > 0) {
			memcpy(faac_flush_buffer, pcm + pcm_pos, sizeof(short) * faac_flush_buffer_size);
		}
	}

	return result;
}

unsigned char* AacEncoder::flush(int* resultSize) {
	*resultSize = -1;
	int enc_size = maxBytesOutput * 2;
	unsigned char* buffer = new unsigned char[enc_size];
	int buffer_pos = 0;

	if (faac_flush_buffer_size) {
		buffer_pos = faacEncEncode(faac, (int32_t *) faac_flush_buffer,
				faac_flush_buffer_size, buffer, enc_size);


		faac_flush_buffer_size = 0;
		if (buffer_pos) {
			if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"flush encode result = %D", buffer_pos);
		} else {
			if (logging) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG,	"flush encode error");
		}
	}

	int enc_result = faacEncEncode(faac, NULL, 0, buffer + buffer_pos, enc_size - buffer_pos);
	if (enc_result >= 0) {
		if (enc_result) {
			buffer_pos += enc_result;
			*resultSize = buffer_pos;
			if (logging) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,	"flush result = %D", enc_result);
			return buffer;
		}
	} else {
		if (logging) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "flush result = %D", enc_result);
		return NULL;
	}
}

void AacEncoder::close() {
	if (initialized) {
		if (faac_flush_buffer) delete[] faac_flush_buffer;
		if (pcmBuf) delete[] pcmBuf;
		faac_flush_buffer_size = 0;
		if (configuration) delete configuration;

		initialized = false;
	}
}
