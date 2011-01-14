package com.shaubert.android.aaf.domain;

import java.io.IOException;

import android.media.AudioRecord;

public class AndroidAudioSource implements AudioSource {

    private AudioRecord androidRecorder;
    
    public AndroidAudioSource(int audioSource, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes) {
        this(new AudioRecord(audioSource, sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes));
    }

    public AndroidAudioSource(AudioRecord androidRecorder) {
        this.androidRecorder = androidRecorder;
    }    
    
    @Override
    public void close() {
        androidRecorder.stop();
    }

    @Override
    public void open() throws IOException {
        androidRecorder.startRecording();
    }

    @Override
    public int read(byte[] buffer, int offset, int count) throws IOException {
        return androidRecorder.read(buffer, offset, count);
    }

}
