package com.shaubert.android.aaf.jni;

import android.util.Log;


public class AudioCoreJNI {

    static {        
        System.loadLibrary("core");
        Log.d(AudioCoreJNI.class.getSimpleName(), "library loaded");
    }
    
    private static AudioCoreJNI instance; 
    
    public static AudioCoreJNI getInstance() {
        if (instance == null) {
            instance = new AudioCoreJNI();
        }
        return instance;
    }
    
    private boolean logging = true;
    private boolean initialized;
    
    private AudioCoreJNI() {    
    }
    
    public void initialize(AudioCoreConfiguration configuration) throws AudioCoreNativeException {
        close();
        if (!nativeInitialize(configuration.getConfiguration())) {
            throw new AudioCoreNativeException("initialization failed");
        } else {
            initialized = true;
            nativeSetLogging(logging);
        }
    }
        
    public boolean isInitialized() {
        return initialized;
    }
    
    public int getOutputBufferSize(int samplesCount) {
        throwIfNotInitialized();
        return nativeGetOutputBufferSize(samplesCount);
    }

    public int encode(byte[] pcm, int offset, int length, byte[] output) throws AudioCoreNativeException {
        throwIfNotInitialized();
        int result = nativeEncode(pcm, offset, length, output);
        if (result >= 0) {
            return result;
        } else {
            throw new AudioCoreNativeException("encoding error");
        }
    }
    
    /**
     * Flush the encoder buffers
     * @return encoded audio data or null
     * @throws AudioCoreNativeException
     */
    public byte[] flush() {
        throwIfNotInitialized();
        return nativeFlush();
    }
    
    public void close() {
        if (initialized) {
            nativeClose();
            initialized = false;
        }
    }
    
    private void throwIfNotInitialized() {
        if (!initialized) {
            throw new IllegalStateException("AudioCoreJNI is not initialized");
        }
    }
    
    public void setLogging(boolean logging) {
        this.logging = logging;
        if (initialized) {
            nativeSetLogging(logging);
        }
    }
    
    private native void nativeSetLogging(boolean on);
    private native boolean nativeInitialize(int[] configuration);
    private native int nativeGetOutputBufferSize(int samplesCount);
    private native int nativeEncode(byte[] pcm, int offset, int length, byte[] output);
    private native byte[] nativeFlush();
    private native void nativeClose();
}
