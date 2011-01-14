package com.shaubert.android.aaf.domain;

import java.util.HashMap;

public class AudioRecorderConfiguration {

	public static final String BITRATE = "bitrate";
	public static final String BANDWIDTH = "bandwidth";	
	public static final String AUDIO_FORMAT = "audio-format";
	public static final String BIT_PER_SAMPLE = "bit-per-sample";
		
	private HashMap<String, Integer> properties;
	
	public AudioRecorderConfiguration() {
		properties = new HashMap<String, Integer>();
	}
	
	public int getValue(String parameter) {
		return properties.get(parameter);
	}
	
	public boolean hasValue(String parameter) {
		return properties.containsKey(parameter);
	}
	
	public int getValue(String parameter, int defaultValue) {
		return hasValue(parameter) ? getValue(parameter) : defaultValue;
	}
	
	public void setValue(String parameter, int value) {
		properties.put(parameter, value);
	}
}
