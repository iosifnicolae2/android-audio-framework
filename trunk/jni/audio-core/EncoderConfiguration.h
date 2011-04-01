#pragma once
class EncoderConfiguration
{

private:
	int inSamplerate;
	int numChannels;
	int quality;
	int mode;
	int bitrate;
	int bitPerSample;

public:
	EncoderConfiguration(void);
	~EncoderConfiguration(void);

	int getInSamplerate();
	void setInSamplerate(int sampleRate);

	int getNumChannels();
	void setNumChannels(int numChannels);

	int getQuality();
	void setQuality(int quality);

	int getMode();
	void setMode(int mode);

	int getBitrate();
	void setBitrate(int bitrate);

	int getBitPerSample();
	void setBitPerSample(int bitPerSample);
};

