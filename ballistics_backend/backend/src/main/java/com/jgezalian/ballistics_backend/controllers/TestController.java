package com.jgezalian.ballistics_backend.controllers;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import com.jgezalian.ballistics_backend.helpers.RecursiveDeleteVisitor;
import java.nio.file.Path;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class TestController {
    @Value("${app.tmp.dir}")
    String tmpDirPathString;

    @GetMapping("test")
    public void testStuff(@AuthenticationPrincipal OAuth2User principal) {

        Path startingDir = Paths.get(tmpDirPathString).resolve("1");
        RecursiveDeleteVisitor del = new RecursiveDeleteVisitor();
        try {
            System.out.println("");
            Files.walkFileTree(startingDir, del);
        } catch (IOException e) {

        }
    }
}
