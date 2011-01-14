package com.shaubert.android.aaf.domain;

import android.content.Context;

public class HumanReadableException extends Exception {

	private static final long serialVersionUID = 8829869418685049818L;
	
	private int messageId;

	public HumanReadableException(String message) {
        super(message);
    }

	public HumanReadableException(String message, Throwable ex) {
        super(message, ex);
    }
	
	public HumanReadableException(int errorStringId) {
		this(errorStringId, null);
	}
	
	public HumanReadableException(int errorStringId, Throwable ex) {
		super(ex);
		this.messageId = errorStringId;
	}
	
	public String getMessage(Context context) {
	    if (messageId == 0) {
	        return getMessage();
	    } else {
	        return context.getString(messageId);
	    }
	}
}
