#pragma once
#include "audioencoder.h"
#include "lame.h"
class LameEncoder :
	public AudioEncoder
{
private:
	lame_t lame;

public:
	LameEncoder(void);
	~LameEncoder(void);

	virtual bool init(EncoderConfiguration* configuration);
	virtual int getBufferSize(int pcmBufferSize);
	virtual int encode(short int* pcm, int length, unsigned char * output);
	virtual unsigned char* flush(int* resultSize);
	virtual void close();
};

