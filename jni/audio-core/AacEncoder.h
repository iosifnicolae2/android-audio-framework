#pragma once
#include "audioencoder.h"
#include "faac.h"

class AacEncoder :
	public AudioEncoder
{
private:
	faacEncHandle faac;
	unsigned long samplesInput;
	unsigned long maxBytesOutput;
	short* faac_flush_buffer;
	int faac_flush_buffer_size;
	short * pcmBuf;

public:
	AacEncoder(void);
	~AacEncoder(void);

	virtual bool init(EncoderConfiguration* configuration);
	virtual int getBufferSize(int pcmBufferSize);
	virtual int encode(short int* pcm, int length, unsigned char * output);
	virtual unsigned char* flush(int* resultSize);
	virtual void close();
};

