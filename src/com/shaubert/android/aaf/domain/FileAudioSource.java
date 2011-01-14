package com.shaubert.android.aaf.domain;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.logging.Logger;

public class FileAudioSource implements AudioSource {
    
    private static Logger LOGGER = Logger.getLogger(FileAudioSource.class.getName());
    
    private File file;
    private FileInputStream input;
    
    public FileAudioSource(File file) {
        this.file = file;
    }
    
    @Override
    public void close() {
        try {
            input.close();
        } catch (IOException e) {
            LOGGER.warning(e.toString());
        }
        input = null;
    }

    @Override
    public void open() throws IOException {
        if (input != null) {
            close();
        }
        input = new FileInputStream(file);
    }

    @Override
    public int read(byte[] buffer, int offset, int count) throws IOException {
        return input.read(buffer, offset, count);
    }

}
