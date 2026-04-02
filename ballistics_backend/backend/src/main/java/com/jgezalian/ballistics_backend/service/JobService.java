package com.jgezalian.ballistics_backend.service;

import java.time.Instant;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.transaction.annotation.Transactional;
import com.jgezalian.ballistics_backend.dto.CreateJobRequest;
import com.jgezalian.ballistics_backend.entity.Job;
import com.jgezalian.ballistics_backend.repository.JobsRepository;
import com.jgezalian.ballistics_backend.entity.Job.JobStatus;
import com.jgezalian.ballistics_backend.exception.JobNotFoundException;
import com.jgezalian.ballistics_backend.exception.RenderException;
import com.jgezalian.ballistics_backend.exception.VideoStorageException;
import com.jgezalian.ballistics_backend.helpers.RecursiveDeleteVisitor;
import com.jgezalian.ballistics_backend.dto.Video;
import java.util.ArrayList;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;
import java.util.Optional;

@Service
public class JobService {
    @Value("${app.tmp.dir}")
    String tmpDirPathString;

    private final JobsRepository jr;
    private final RenderService rs;
    private final VideoStorageService vs;
    private final JobExecutorService je;

    JobService(JobsRepository jr, RenderService rs, VideoStorageService vs, JobExecutorService je) {
        this.jr = jr;
        this.rs = rs;
        this.vs = vs;
        this.je = je;
    }

    public Job createQueuedJob(CreateJobRequest jobRequest, String userId) {
        Job job = new Job();
        job.setVx(jobRequest.getVx());
        job.setVy(jobRequest.getVy());
        job.setVz(jobRequest.getVz());
        job.setUserId(userId);
        job.setJobStatus(JobStatus.QUEUED);
        job.setCreatedAt(Instant.now());
        Job savedJob = jr.save(job);
        je.submit(() -> processJob(savedJob.getId()));
        return savedJob;
    }

    public List<Job> getUserJobs(String userId) {
        return jr.findByUserIdOrderByCreatedAtDesc(userId);
    }

    public Optional<Job> getUserJob(String userId, Long id) {
        return jr.findByUserIdAndId(userId, id);
    }

    public Optional<Job> getUserCompletedJob(String userId, Long id) {
        return jr.findByUserIdAndIdAndJobStatus(userId, id, JobStatus.COMPLETED);
    }

    public List<Job> getUserCompletedJobs(String userId) {
        return jr.findByUserIdAndJobStatusOrderByCreatedAtDesc(userId, JobStatus.COMPLETED);
    }

    public Video getUserVideo(String userId, Long id) {
        Job job = getUserCompletedJob(userId, id).orElseThrow(() -> new JobNotFoundException(id));
        return new Video(job.getId(), vs.createPresignedGetUrl(job.getS3Key()));
    }

    public List<Video> getUserVideos(String userId) {
        List<Video> userVideos = new ArrayList<>();
        List<Job> completedJobs = getUserCompletedJobs(userId);
        completedJobs.forEach((job) -> {
            userVideos.add(new Video(job.getId(), vs.createPresignedGetUrl(job.getS3Key())));
        });
        return userVideos;
    }

    public void processJob(Long id) {
        Job job = jr.findById(id).orElseThrow(() -> new JobNotFoundException(id));
        if (job.getJobStatus() != Job.JobStatus.QUEUED) {
            return;
        }

        try {
            job.setJobStatus(JobStatus.RUNNING);
            job.setStartedAt(Instant.now());
            jr.save(job);

            Path videoPath = rs.renderProcess(job.getVx(), job.getVy(), job.getVz(), job.getId());
            String s3Key = vs.storeVideo(job.getId(), job.getUserId(), videoPath);

            job.setJobStatus(JobStatus.COMPLETED);
            job.setFinishedAt(Instant.now());
            job.setS3Key(s3Key);
            jr.save(job);
        }

        catch (RenderException e) {
            job.setJobStatus(JobStatus.FAILED);
            job.setFinishedAt(Instant.now());
            job.setErrorMsg(e.getMessage());
            jr.save(job);
        }

        catch (VideoStorageException e) {
            job.setJobStatus(JobStatus.FAILED);
            job.setFinishedAt(Instant.now());
            job.setErrorMsg(e.getMessage());
            jr.save(job);
        }

        catch (Exception e) {
            job.setJobStatus(JobStatus.FAILED);
            job.setFinishedAt(Instant.now());
            job.setErrorMsg("Unexpected failure: " + e.getMessage());
            jr.save(job);
        }

        finally {
            Path startingDir = Paths.get(tmpDirPathString).resolve(id.toString());
            RecursiveDeleteVisitor del = new RecursiveDeleteVisitor();
            try {
                if (Files.exists(startingDir)) {
                    Files.walkFileTree(startingDir, del);
                }

            } catch (IOException e) {
                System.err.println("Failed to delete tmp storage directory for job: " + id + " " + e.getMessage());
            }
        }

    }

    @Transactional
    public void deleteUserJob(Long id, String userId) {
        Job job = jr.findByUserIdAndId(userId, id).orElseThrow(() -> new JobNotFoundException(id));
        if (job.getJobStatus() == JobStatus.QUEUED || job.getJobStatus() == JobStatus.RUNNING) {
            // throw new VideoStorageException("Unable to delete job");
            throw new ResponseStatusException(HttpStatus.CONFLICT, "Cannot delete queued or running job");
        }
        vs.deleteVideo(id, userId);
        jr.deleteByUserIdAndId(userId, id);
    }
}
