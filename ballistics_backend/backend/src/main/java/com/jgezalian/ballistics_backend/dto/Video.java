package com.jgezalian.ballistics_backend.dto;

public class Video {
    Long id;
    String url;

    public Video(Long id, String url) {
        this.id = id;
        this.url = url;
    }

    public Long getId() {
        return id;
    }

    public String getUrl() {
        return url;
    }
}
