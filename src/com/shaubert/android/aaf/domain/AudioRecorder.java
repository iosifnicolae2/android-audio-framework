package com.shaubert.android.aaf.domain;

import java.io.OutputStream;

public interface AudioRecorder {

	enum State { 
		STANDBY, INITIALIZED, RECORDING, PAUSED, STOPPING, RELEASED, ERROR
	}
	
	interface AudioRecorderListener {
		void onStateChanged(AudioRecorder recorder, State newState);
	}
	
	void initialize(OutputStream output, AudioSource input, AudioRecorderConfiguration configuration) throws HumanReadableException;
	
	void record() throws HumanReadableException;
	void pause() throws HumanReadableException;
	void resume() throws HumanReadableException;
	void stop() throws HumanReadableException;
		
	HumanReadableException getLastError();
}
