
package com.shaubert.android.aaf.domain;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

import com.shaubert.android.aaf.R;

import android.os.Handler;

public class DefaultAudioRecorder implements AudioRecorder {

    protected static final Logger LOGGER = Logger.getLogger(AudioRecorder.class.getName()); 
    
    private Object locker;
    
    private State state;
    private HumanReadableException lastError;
    private AudioSource source;
    private OutputStream output;
    private AudioRecorderConfiguration configuration;
    private AudioEncoder encoder;
    private EncodingThread encodingThread;
    private Handler handler;
    
    private class EncodingThread extends Thread {
        private volatile boolean stopRequest;
        private Runnable onStop;
        private volatile boolean silent;
        
        public void run() {
            byte[] rawData = new byte[encoder.getInputBufferSize()];
            try {
                while (!isInterrupted() && (state == AudioRecorder.State.RECORDING || state == AudioRecorder.State.PAUSED)) {
                    try {
                        if (stopRequest) {
                            byte[] encoded = encoder.flushAndClose();
                            output.write(encoded);
                            if (onStop != null) {
                                handler.post(onStop);
                            }
                        } else if (state != AudioRecorder.State.PAUSED) {
                            int len = source.read(rawData, 0, rawData.length);
                            if (len > 0) {
                                byte[] encoded = encoder.encode(rawData, 0, len);
                                output.write(encoded);
                            } else if (len < 0) {
                                stop();
                            }
                        } else {
                            try {
                                sleep(50);
                            } catch (InterruptedException e) { }
                        }
                    } catch (IOException e) {
                        throw new HumanReadableException(R.string.audio_data_read_error, e);
                    }
                }
            } catch (HumanReadableException ex) {
                if (!silent) {
                    handleEncodingException(ex);
                }
            }
        }
        
        public void stopRequest(Runnable onStop) {
            this.onStop = onStop;
            stopRequest = true;
            if (!isAlive()) {
                onStop.run();
            }
        }
        
        public void forceStop() {
            this.onStop = null;
            silent = true;
        }
    }
    
    private List<AudioRecorderListener> listeners;

    public DefaultAudioRecorder() {
        this.locker = new Object();
        this.listeners = new ArrayList<AudioRecorderListener>();
        this.handler = new Handler();
    }
    
    public void addListener(AudioRecorderListener listener) {
        this.listeners.add(listener);
    }
    
    public void removeListener(AudioRecorderListener listener) {
        this.listeners.remove(listener);
    }
        
    private Runnable raiseStateChange = new Runnable() {
        public void run() {
            for (AudioRecorderListener listener : listeners) {
                listener.onStateChanged(DefaultAudioRecorder.this, state);
            }
        }
    };
    
    protected void setState(State state) {
        this.state = state;
        if (Thread.currentThread() != handler.getLooper().getThread()) {
            handler.post(raiseStateChange);
        } else {
            raiseStateChange.run();
        }
    }
    
    public State getState() {
        return state;
    }
    
    @Override
    public HumanReadableException getLastError() {
        return lastError;
    }

    private void handleEncodingException(HumanReadableException ex) {
        lastError = ex;
        setState(State.ERROR);                
    }
    
    @Override
    public void initialize(OutputStream output, AudioSource input, AudioRecorderConfiguration configuration) throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.STANDBY, State.ERROR, State.INITIALIZED);
            this.output = output;
            this.source = input;
            this.configuration = configuration;
            this.encoder = AudioEncoderFactory.getInstance().createEncoder(configuration);
            this.encoder.initialize(this.configuration);
            setState(State.INITIALIZED);
        }
    }

    @Override
    public void pause() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.RECORDING, State.PAUSED);
            setState(State.PAUSED);
        }
    }
    
    @Override
    public void resume() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.PAUSED);
            setState(State.RECORDING);
        }
    }

    @Override
    public void record() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.INITIALIZED);
            encodingThread = new EncodingThread();
            encodingThread.start();
            setState(State.RECORDING);
        }
    }

    private Runnable stopAction = new Runnable() {
        public void run() {
            try {
                output.close();
            } catch (IOException e) {
                LOGGER.warning(e.toString());
            }
            source.close();
            setState(State.STANDBY);            
        }
    };
    
    @Override
    public void stop() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.RECORDING, State.PAUSED);
            if (!encodingThread.stopRequest) {
                encodingThread.stopRequest(stopAction);
                setState(State.STOPPING);
            } else {                
                encodingThread.forceStop();
                encodingThread.interrupt();
                handler.removeCallbacks(stopAction);
                stopAction.run();
            }
        }    
    }
    
    protected String statesToString(State ... states) {
        if (states.length > 0) {
            StringBuilder builder = new StringBuilder();
            for (State state : states) {
                builder.append(state.name() + ", ");
            }
            builder.delete(builder.length() - 2, builder.length());
            return builder.toString();
        } else {
            return "";
        }        
    }
    
    protected void throwIfStateNotAllowed(State ... allowedStates) throws HumanReadableException {
        for (State allowed : allowedStates) {
            if (allowed == state) {
                return;
            }
        }
        throw (lastError = new HumanReadableException(String.format("Audio recorder state %s is not in allowed list [%s]", 
                state.name(), statesToString(allowedStates))));
    }
    
}
