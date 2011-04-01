package com.shaubert.android.aaf.jni;

public class AudioCoreJNI {

    public static native void setLogging(boolean on);
    
    public static native int getMinimumBufferSize(int pcmBufferSizeInShorts);
    public static native int encodePCM(byte[] pcm, int offset, int length, byte[] result);    
    public static native void close();
    
    //LAME
    public static native void lameInit(int inSamplerate, int numChannels, int quality, int mode, int bitrate);
    public static native byte[] lameFlush();
    
    //FAAC
    public static native int faacInit(int inSamplerate, int bitrate, int quality);    
    public static native byte[] faacFlush();
    
    //OGG
}
