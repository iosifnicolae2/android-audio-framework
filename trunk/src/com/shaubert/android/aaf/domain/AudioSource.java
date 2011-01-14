package com.shaubert.android.aaf.domain;

import java.io.IOException;

public interface AudioSource {

	void open() throws IOException;
	void close();
	
	int read(byte[] buffer, int offset, int count) throws IOException;
}
