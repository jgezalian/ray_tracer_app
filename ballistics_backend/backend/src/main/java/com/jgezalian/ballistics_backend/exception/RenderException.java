package com.jgezalian.ballistics_backend.exception;

public class RenderException extends RuntimeException {
    public RenderException(String message) {
        super(message);
    }

    public RenderException(String message, Throwable e){
        super(message, e);
    }
}
