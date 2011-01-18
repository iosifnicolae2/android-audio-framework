package com.shaubert.android.aaf.domain;

public interface AudioEncoder {

    void initialize(AudioRecorderConfiguration configuration) throws HumanReadableException;
    int getInputBufferSize();
    int getMaxOutputInBytes();
    byte[] encode(byte[] pcm, int offset, int length) throws HumanReadableException;
    byte[] flushAndClose() throws HumanReadableException;
    
}
