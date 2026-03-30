package com.jgezalian.ballistics_backend.service;

import software.amazon.awssdk.core.async.AsyncRequestBody;
import software.amazon.awssdk.services.s3.S3AsyncClient;
import software.amazon.awssdk.services.s3.model.GetObjectRequest;
import software.amazon.awssdk.services.s3.model.PutObjectRequest;
import software.amazon.awssdk.services.s3.model.DeleteObjectRequest;
import software.amazon.awssdk.services.s3.presigner.S3Presigner;
import software.amazon.awssdk.services.s3.presigner.model.GetObjectPresignRequest;
import software.amazon.awssdk.services.s3.presigner.model.PresignedGetObjectRequest;

import org.springframework.stereotype.Service;

import com.jgezalian.ballistics_backend.exception.VideoStorageException;

import org.springframework.beans.factory.annotation.Value;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.logging.Logger;
import java.time.Duration;

@Service
public class VideoStorageService {

    public VideoStorageService() {

    }

    @Value("${app.s3.bucket}")
    private String bucketName;
    S3AsyncClient s3Client = S3AsyncClient.crtCreate();
    private static final Logger logger = Logger.getLogger(VideoStorageService.class.getName());

    public String storeVideo(Long jobId, String userId, Path videoPath) {
        String s3Key = userId + "/" + jobId + "/render.mp4";
        try {
            PutObjectRequest objectRequest = PutObjectRequest.builder()
                    .bucket(bucketName)
                    .key(s3Key)
                    .build();

            s3Client
                    .putObject(objectRequest,
                            AsyncRequestBody.fromFile(videoPath))
                    .join();
        }

        catch (Exception e) {
            throw new VideoStorageException("Failed to put object", e);
        }

        try {
            Files.deleteIfExists(videoPath);
        } catch (IOException e) {
            throw new VideoStorageException("Could not delete tmp video", e);
        }

        return s3Key;

    }

    public void deleteVideo(Long jobId, String userId) {
        String s3Key = userId + "/" + jobId + "/render.mp4";
        try {
            DeleteObjectRequest delRequest = DeleteObjectRequest.builder()
                    .bucket(bucketName)
                    .key(s3Key)
                    .build();

            s3Client
                    .deleteObject(delRequest).join();
        } catch (Exception e) {
            throw new VideoStorageException("Failed to delete object: ", e);
        }
    }

    // public String getURL(String bucketName, String keyName) {
    // try {
    // GetUrlRequest request = GetUrlRequest.builder()
    // .bucket(bucketName)
    // .key(keyName)
    // .build();

    // URL url = s3Client.utilities().getUrl(request);
    // return url.toString();

    // } catch (S3Exception e) {
    // System.err.println(e.awsErrorDetails().errorMessage());
    // }
    // return "";

    // }

    public String createPresignedGetUrl(String keyName) {
        try (S3Presigner presigner = S3Presigner.create()) {

            GetObjectRequest objectRequest = GetObjectRequest.builder()
                    .bucket(bucketName)
                    .key(keyName)
                    .build();

            GetObjectPresignRequest presignRequest = GetObjectPresignRequest.builder()
                    .signatureDuration(Duration.ofMinutes(120))
                    .getObjectRequest(objectRequest)
                    .build();

            PresignedGetObjectRequest presignedRequest = presigner.presignGetObject(presignRequest);
            // logger.info(presignedRequest.url().toString());

            return presignedRequest.url().toExternalForm();
        } catch (Exception e) {
            throw new VideoStorageException("Failed to fetch presigned url for key: " + keyName, e);
        }
    }

}