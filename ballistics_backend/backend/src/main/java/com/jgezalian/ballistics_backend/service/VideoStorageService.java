package com.jgezalian.ballistics_backend.service;

import software.amazon.awssdk.core.async.AsyncRequestBody;
import software.amazon.awssdk.core.async.AsyncResponseTransformer;
import software.amazon.awssdk.services.s3.S3AsyncClient;
import software.amazon.awssdk.services.s3.model.GetObjectResponse;
import software.amazon.awssdk.services.s3.model.PutObjectResponse;
import java.nio.file.Paths;
import org.springframework.stereotype.Service;
import org.springframework.beans.factory.annotation.Value;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

@Service
public class VideoStorageService {

    public VideoStorageService() {

    }

    @Value("${app.s3.bucket}")
    private String bucket_name;

    // @Value("${app.tmp.dir.renders}")
    // private String tmp_dir_path_renders_string;

    public void storeVideo(String job_id, String user_id, Path job_dir) {
        S3AsyncClient s3Client = S3AsyncClient.crtCreate();

        PutObjectResponse putObjectResponse = s3Client
                .putObject(req -> req.bucket(bucket_name)
                        .key(user_id + "/" + job_id),
                        AsyncRequestBody.fromFile(job_dir.resolve("render.mp4")))
                .join();

        try {
            Files.deleteIfExists(job_dir.resolve("render.mp4"));
            Files.deleteIfExists(job_dir);
        } catch (IOException e) {
            System.err.println("Caught IOException: " + e.getMessage());
        }

        // Download an object from Amazon S3 to a local file.
        // GetObjectResponse getObjectResponse =
        // s3Client.getObject(req -> req.bucket(<BUCKET_NAME>)
        // .key(<KEY_NAME>),
        // AsyncResponseTransformer.toFile(Paths.get(<FILE_NAME>)))
        // .join();
        // S3AsyncClient.crtBuilder()
        // .credentialsProvider(DefaultCredentialsProvider.create())
        // .region(Region.US_WEST_2)
        // .targetThroughputInGbps(20.0)
        // .minimumPartSizeInBytes(8 * 1025 * 1024L)
        // .build();

        // try {
        // Files.deleteIfExists(job_dir);
        // } catch (IOException e) {
        // System.err.println("Caught IOException: " + e.getMessage());
        // }

    }
}