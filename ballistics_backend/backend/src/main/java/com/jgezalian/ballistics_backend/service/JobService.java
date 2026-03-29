package com.jgezalian.ballistics_backend.service;

import java.time.Instant;
import org.springframework.stereotype.Service;
import com.jgezalian.ballistics_backend.entity.Job;
import com.jgezalian.ballistics_backend.entity.SceneParam;
import com.jgezalian.ballistics_backend.repository.JobsRepository;
import com.jgezalian.ballistics_backend.entity.Job.JobStatus;
import com.jgezalian.ballistics_backend.exception.JobNotFoundException;
import com.jgezalian.ballistics_backend.exception.RenderException;
import com.jgezalian.ballistics_backend.exception.VideoStorageException;
import java.util.ArrayList;
import java.nio.file.Path;
import java.util.List;
import java.util.Optional;

@Service
public class JobService {

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

    public Job createQueuedJob(SceneParam sp, String userId) {
        Job job = new Job();
        job.setVx(sp.getVx());
        job.setVy(sp.getVy());
        job.setUserId(userId);
        job.setJobStatus(JobStatus.QUEUED);
        job.setCreatedAt(Instant.now());
        jr.save(job);
        je.submit(() -> processJob(job.getId()));
        return job;
    }

    public List<Job> getUserJobs(String userId) {
        return jr.findByUserIdOrderByCreatedAtDesc(userId);
    }

    public Optional<Job> getUserJob(String userId, Long id) {
        return jr.findByUserIdAndId(userId, id);
    }

    public List<Job> getUserCompletedJobs(String userId) {
        return jr.findByUserIdAndJobStatusOrderByCreatedAtDesc(userId, JobStatus.COMPLETED);
    }

    public List<String> getUserRenders(String userId) {
        List<String> renderUrls = new ArrayList<>();
        List<Job> completedJobs = getUserCompletedJobs(userId);
        completedJobs.forEach((job) -> {
            renderUrls.add(vs.createPresignedGetUrl(job.getS3Key()));
        });
        return renderUrls;
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

            Path videoPath = rs.renderProcess(job.getVx(), job.getVy(), job.getUserId(), job.getId());
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

    }

}
