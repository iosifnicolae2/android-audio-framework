#include "EncoderConfiguration.h"


EncoderConfiguration::EncoderConfiguration(void)
	: inSamplerate(-1), numChannels(-1), quality(-1), mode(-1), bitrate(-1)
{
}

EncoderConfiguration::~EncoderConfiguration(void)
{
}

int EncoderConfiguration::getInSamplerate() 
{
	return inSamplerate;
}

void EncoderConfiguration::setInSamplerate(int sampleRate) 
{
	inSamplerate = sampleRate;
}

int EncoderConfiguration::getBitrate() 
{
	return bitrate;
}

void EncoderConfiguration::setBitrate(int bitrate)
{
	this->bitrate = bitrate;
}

int EncoderConfiguration::getMode() 
{
	return mode;
}

void EncoderConfiguration::setMode(int mode)
{
	this->mode = mode;
}

int EncoderConfiguration::getNumChannels() 
{
	return numChannels;
}

void EncoderConfiguration::setNumChannels(int numChannels)
{
	this->numChannels = numChannels;
}

int EncoderConfiguration::getQuality() 
{
	return quality;
}

void EncoderConfiguration::setQuality(int quality)
{
	this->quality = quality;
}

int EncoderConfiguration::getBitPerSample() {
	return bitPerSample;
}

void EncoderConfiguration::setBitPerSample(int bitPerSample) {
	this->bitPerSample = bitPerSample;
}