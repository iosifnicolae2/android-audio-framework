
package com.shaubert.android.aaf.domain;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class DefaultAudioRecorder implements AudioRecorder {

    protected static final Logger LOGGER = Logger.getLogger(AudioRecorder.class.getName());
    
    private Object locker;
    
    private State state;
    private HumanReadableException lastError;
    private AudioSource source;
    private OutputStream output;
    private AudioRecorderConfiguration configuration;
    
    private List<AudioRecorderListener> listeners;

    public DefaultAudioRecorder() {
        this.locker = new Object();
        this.listeners = new ArrayList<AudioRecorderListener>();
    }
    
    public void addListener(AudioRecorderListener listener) {
        this.listeners.add(listener);
    }
    
    public void removeListener(AudioRecorderListener listener) {
        this.listeners.remove(listener);
    }
    
    protected void setState(State state) {
        this.state = state;
        for (AudioRecorderListener listener : listeners) {
            listener.onStateChanged(this, state);
        }
    }
    
    public State getState() {
        return state;
    }
    
    @Override
    public HumanReadableException getLastError() {
        return lastError;
    }

    @Override
    public void initialize(OutputStream output, AudioSource input, AudioRecorderConfiguration configuration) throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.STANDBY, State.ERROR, State.INITIALIZED);
            this.output = output;
            this.source = input;
            this.configuration = configuration;            
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
    public void record() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.INITIALIZED);
            setState(State.RECORDING);
        }
    }

    @Override
    public void release() throws HumanReadableException {
        synchronized (locker) {
            reset();
            setState(State.RELEASED);
        }
    }

    @Override
    public void reset() throws HumanReadableException {
        synchronized (locker) {
            if (state == State.RECORDING || state == State.PAUSED) {
                stop();
            }
            setState(State.STANDBY);
        }    
    }

    @Override
    public void stop() throws HumanReadableException {
        synchronized (locker) {
            throwIfStateNotAllowed(State.RECORDING, State.PAUSED);
            try {
                output.close();
            } catch (IOException e) {
                LOGGER.warning(e.toString());
            }
            source.close();
            setState(State.STANDBY);
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
