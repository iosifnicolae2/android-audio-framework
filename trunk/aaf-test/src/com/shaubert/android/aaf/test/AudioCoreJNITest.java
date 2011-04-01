package com.shaubert.android.aaf.test;

import com.shaubert.android.aaf.jni.AudioCoreConfiguration;
import com.shaubert.android.aaf.jni.AudioCoreJNI;
import com.shaubert.android.aaf.jni.AudioCoreConfiguration.EncoderType;

import android.test.AndroidTestCase;

public class AudioCoreJNITest extends AndroidTestCase {

    
    public void testInitializingAAC() throws Exception {
        AudioCoreJNI coreJNI = AudioCoreJNI.getInstance();
        coreJNI.initialize(new AudioCoreConfiguration(EncoderType.AAC));
        coreJNI.close();
    }
    
    public void testClosing() throws Exception {
        AudioCoreJNI coreJNI = AudioCoreJNI.getInstance();
        coreJNI.initialize(new AudioCoreConfiguration(EncoderType.AAC));
        coreJNI.close();
        coreJNI.initialize(new AudioCoreConfiguration(EncoderType.AAC));
        coreJNI.close();
    }
    
    public void testInitializingMP3() throws Exception {
        AudioCoreJNI coreJNI = AudioCoreJNI.getInstance();
        coreJNI.initialize(new AudioCoreConfiguration(EncoderType.MP3));
        coreJNI.close();
    }
}
