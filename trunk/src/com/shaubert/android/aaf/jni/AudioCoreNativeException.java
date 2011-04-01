package com.shaubert.android.aaf.jni;

public class AudioCoreNativeException extends Exception {

    private static final long serialVersionUID = 6097272652529062637L;

    public AudioCoreNativeException() {
        super();
    }

    public AudioCoreNativeException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public AudioCoreNativeException(String detailMessage) {
        super(detailMessage);
    }

    public AudioCoreNativeException(Throwable throwable) {
        super(throwable);
    }
    
    
}
