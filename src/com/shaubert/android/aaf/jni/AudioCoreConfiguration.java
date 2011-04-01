package com.shaubert.android.aaf.jni;


public class AudioCoreConfiguration {
    
    public enum EncoderType {
        AAC(1),
        MP3(2),
        OGG(3);
        
        final int code;
        
        private EncoderType(int code) {
            this.code = code;
        }
    }
    
    private static final int ENCODER_TYPE_POSITION = 0;
    private static final int IN_SAMPLERATE_POSITION = 1;
    private static final int NUM_CHANNELS_POSITION = 2;
    private static final int QUALITY_POSITION = 3;
    private static final int MODE_POSITION = 4;
    private static final int BITRATE_POSITION = 5;
    private static final int BIT_PER_SAMPLE_POSITION = 6;    
    private static final int SIZE = 7;
    
    private int[] configuration;
    private EncoderType encoderType;
    
    public AudioCoreConfiguration(EncoderType encoderType) {
        configuration = new int[SIZE];
        configuration[ENCODER_TYPE_POSITION] = encoderType.code;
        this.encoderType = encoderType;
    }
        
    int[] getConfiguration() {
        return configuration;
    }
    
    public EncoderType getEncoderType() {
        return encoderType;
    }
    
    public int getInSamplerate() {
        return configuration[IN_SAMPLERATE_POSITION];
    }

    public void setInSamplerate(int sampleRate) {
        configuration[IN_SAMPLERATE_POSITION] = sampleRate;
    }

    public int getBitrate() {
        return configuration[BITRATE_POSITION];
    }

    public void setBitrate(int bitrate) {
        configuration[BITRATE_POSITION] = bitrate;
    }

    public int getMode() 
    {
        return configuration[MODE_POSITION];
    }

    public void setMode(int mode) {
        configuration[MODE_POSITION] = mode;
    }

    public int getNumChannels() 
    {
        return configuration[NUM_CHANNELS_POSITION];
    }

    public void setNumChannels(int numChannels)
    {
        configuration[NUM_CHANNELS_POSITION] = numChannels;
    }

    public int getQuality() 
    {
        return configuration[QUALITY_POSITION];
    }

    public void setQuality(int quality)
    {
        configuration[QUALITY_POSITION] = quality;
    }

    public int getBitPerSample() {
        return configuration[BIT_PER_SAMPLE_POSITION];
    }

    public void setBitPerSample(int bitPerSample) {
        configuration[BIT_PER_SAMPLE_POSITION] = bitPerSample;
    }    
}
