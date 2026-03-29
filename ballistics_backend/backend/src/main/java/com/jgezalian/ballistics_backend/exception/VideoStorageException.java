package com.jgezalian.ballistics_backend.exception;

public class VideoStorageException extends RuntimeException {
    public VideoStorageException(String message) {
        super(message);
    }

    public VideoStorageException(String message, Throwable e) {
        super(message, e);
    }
}
