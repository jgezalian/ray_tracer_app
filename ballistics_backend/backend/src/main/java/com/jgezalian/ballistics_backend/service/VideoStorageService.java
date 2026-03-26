package com.jgezalian.ballistics_backend.service;

import software.amazon.awssdk.core.async.AsyncRequestBody;
import software.amazon.awssdk.core.async.AsyncResponseTransformer;
import software.amazon.awssdk.services.s3.S3AsyncClient;
import software.amazon.awssdk.services.s3.model.GetObjectRequest;
import software.amazon.awssdk.services.s3.model.GetObjectResponse;
import software.amazon.awssdk.services.s3.model.GetUrlRequest;
import software.amazon.awssdk.services.s3.model.ListObjectsV2Request;
import software.amazon.awssdk.services.s3.model.PutObjectResponse;
import software.amazon.awssdk.services.s3.model.S3Exception;
import software.amazon.awssdk.services.s3.paginators.ListObjectsV2Publisher;
import software.amazon.awssdk.services.s3.presigner.S3Presigner;
import software.amazon.awssdk.services.s3.presigner.model.GetObjectPresignRequest;
import software.amazon.awssdk.services.s3.presigner.model.PresignedGetObjectRequest;

import java.nio.file.Paths;
import java.util.concurrent.CompletableFuture;

import org.springframework.stereotype.Service;
import org.springframework.beans.factory.annotation.Value;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.ArrayList;
import java.time.Duration;
import java.util.List;

@Service
public class VideoStorageService {

    public VideoStorageService() {

    }

    @Value("${app.s3.bucket}")
    private String bucket_name;
    S3AsyncClient s3Client = S3AsyncClient.crtCreate();
    private static final Logger logger = Logger.getLogger(VideoStorageService.class.getName());

    // @Value("${app.tmp.dir.renders}")
    // private String tmp_dir_path_renders_string;

    public void storeVideo(String job_id, String user_id, Path job_dir) {

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

    }

    public String getURL(String bucketName, String keyName) {
        try {
            GetUrlRequest request = GetUrlRequest.builder()
                    .bucket(bucketName)
                    .key(keyName)
                    .build();

            URL url = s3Client.utilities().getUrl(request);
            return url.toString();

        } catch (S3Exception e) {
            System.err.println(e.awsErrorDetails().errorMessage());
            System.exit(1);
        }
        return "";

    }

    public String createPresignedGetUrl(String bucketName, String keyName) {
        try (S3Presigner presigner = S3Presigner.create()) {

            GetObjectRequest objectRequest = GetObjectRequest.builder()
                    .bucket(bucketName)
                    .key(keyName)
                    .build();

            GetObjectPresignRequest presignRequest = GetObjectPresignRequest.builder()
                    .signatureDuration(Duration.ofMinutes(120)) // The URL will expire in 10 minutes.
                    .getObjectRequest(objectRequest)
                    .build();

            PresignedGetObjectRequest presignedRequest = presigner.presignGetObject(presignRequest);
            // logger.info(presignedRequest.url().toString());

            return presignedRequest.url().toExternalForm();
        }
    }

    public CompletableFuture<List<String>> getUserObjectUrls(String bucket_name, String user_id) {

        List<String> objects = new ArrayList<>();
        return listAllObjectsAsync(bucket_name, user_id, objects).thenApply(
                (response) -> {
                    List<String> urls = new ArrayList<>();
                    for (String object : response) {
                        urls.add(createPresignedGetUrl(bucket_name, object));
                    }
                    return urls;
                });

    }

    public CompletableFuture<List<String>> listAllObjectsAsync(String bucketName, String userId,
            List<String> objects) {
        ListObjectsV2Request initialRequest = ListObjectsV2Request.builder()
                .bucket(bucketName)
                .maxKeys(1)
                .prefix(userId + "/")
                .delimiter("/")
                .build();
        ListObjectsV2Publisher paginator = s3Client.listObjectsV2Paginator(initialRequest);
        return paginator.subscribe(response -> {
            response.contents().forEach(s3Object -> {
                objects.add(s3Object.key());
                // logger.info("Object key: " + s3Object.key());

            });
        }).thenRun(() -> {
            logger.info("Successfully listed all objects in the bucket: " + bucketName);
        }).exceptionally(ex -> {
            throw new RuntimeException("Failed to list objects", ex);
        }).thenApply(ignored -> List.copyOf(objects));
    }

    public CompletableFuture<List<String>> listRendersRequest(String bucket_name, String user_id) {
        return getUserObjectUrls(bucket_name, user_id);
    }
}