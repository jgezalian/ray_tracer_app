package com.jgezalian.ballistics_backend.helpers;

import static java.nio.file.FileVisitResult.*;

import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.nio.file.Path;
import java.nio.file.Files;

public class RecursiveDeleteVisitor extends SimpleFileVisitor<Path> {


    public RecursiveDeleteVisitor() {
    }

    @Override
    public FileVisitResult visitFile(Path file,
            BasicFileAttributes attr) throws IOException {
        Files.deleteIfExists(file);
        return CONTINUE;
    }

    @Override
    public FileVisitResult postVisitDirectory(Path dir,
            IOException exc) throws IOException {
        if (exc != null) {
            throw exc;
        }

        Files.deleteIfExists(dir);
        return CONTINUE;
    }

    @Override
    public FileVisitResult visitFileFailed(Path file,
            IOException exc) throws IOException {
        throw exc;

    }
}
