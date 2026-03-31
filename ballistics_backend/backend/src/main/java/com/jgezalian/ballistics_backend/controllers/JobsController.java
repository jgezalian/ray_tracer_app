package com.jgezalian.ballistics_backend.controllers;

import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.http.HttpStatus;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PathVariable;
import com.jgezalian.ballistics_backend.entity.Job;
import com.jgezalian.ballistics_backend.service.JobService;
import com.jgezalian.ballistics_backend.exception.JobNotFoundException;
import com.jgezalian.ballistics_backend.dto.CreateJobRequest;
import com.jgezalian.ballistics_backend.dto.Video;
import java.util.List;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class JobsController {

    private final JobService js;

    public JobsController(JobService js) {
        this.js = js;
    }

    @PostMapping("jobs")
    public Job queueJob(@AuthenticationPrincipal OAuth2User principal,
            @RequestBody CreateJobRequest newJobRequest) {
        if (newJobRequest.getVx() == null || newJobRequest.getVy() == null) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "vX and vY are required");
        }
        if (!Double.isFinite(newJobRequest.getVx()) || !Double.isFinite(newJobRequest.getVy())) {
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "vX and vY must be finite");
        }
        String userId = principal.getAttribute("sub");
        return js.createQueuedJob(newJobRequest, userId);
    }

    @GetMapping("jobs")
    public List<Job> getUserJobs(@AuthenticationPrincipal OAuth2User principal) {
        String userId = principal.getAttribute("sub");
        return js.getUserJobs(userId);
    }

    @GetMapping("jobs/{id}")
    public Job getUserJob(@AuthenticationPrincipal OAuth2User principal, @PathVariable("id") Long id) {
        String userId = principal.getAttribute("sub");
        return js.getUserJob(userId, id).orElseThrow(() -> new JobNotFoundException(id));
    }

    @GetMapping("/list_renders")
    public List<Video> listRenders(@AuthenticationPrincipal OAuth2User principal) {
        String userId = principal.getAttribute("sub");
        return js.getUserVideos(userId);
    }

    @GetMapping("get_render/{id}")
    public Video getRender(@AuthenticationPrincipal OAuth2User principal, @PathVariable("id") Long id) {
        String userId = principal.getAttribute("sub");
        return js.getUserVideo(userId, id);
    }

    @DeleteMapping("/jobs/{id}")
    public void deleteUserJob(@AuthenticationPrincipal OAuth2User principal, @PathVariable("id") Long id) {
        String userId = principal.getAttribute("sub");
        js.deleteUserJob(id, userId);
    }

}
