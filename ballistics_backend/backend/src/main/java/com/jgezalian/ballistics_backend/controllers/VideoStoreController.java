package com.jgezalian.ballistics_backend.controllers;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.concurrent.CompletableFuture;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.web.bind.annotation.CrossOrigin;
import com.jgezalian.ballistics_backend.service.VideoStorageService;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import java.util.List;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class VideoStoreController {
    private final VideoStorageService vs;
    @Value("${app.s3.bucket}")
    private String bucketName;

    public VideoStoreController(VideoStorageService vs) {
        this.vs = vs;
    }

}
