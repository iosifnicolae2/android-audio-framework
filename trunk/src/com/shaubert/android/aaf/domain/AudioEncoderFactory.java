package com.shaubert.android.aaf.domain;

public class AudioEncoderFactory {

    private static AudioEncoderFactory instance;
    
    private AudioEncoderFactory() {        
    }
    
    public static AudioEncoderFactory getInstance() {
        if (instance == null) {
            instance = new AudioEncoderFactory();
        }
        return instance;
    }
    
    public AudioEncoder createEncoder(AudioRecorderConfiguration configuration) throws IllegalArgumentException {
        int encoder = configuration.getValue(AudioRecorderConfiguration.ENCODER);
        switch (encoder) {
            default: 
                throw new IllegalArgumentException(String.format("Audio encoder \"%d\" is not supported", encoder));
        }
    }
}
