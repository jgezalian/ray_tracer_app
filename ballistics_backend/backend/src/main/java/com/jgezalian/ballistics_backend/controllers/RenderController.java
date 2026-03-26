package com.jgezalian.ballistics_backend.controllers;

import java.util.List;
import com.jgezalian.ballistics_backend.entity.SceneParam;
import com.jgezalian.ballistics_backend.repository.SceneParamRepository;

import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import com.jgezalian.ballistics_backend.service.RenderService;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import java.util.Map;
import java.util.Collections;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class RenderController {
    private final SceneParamRepository repository;
    private final RenderService rs;

    RenderController(SceneParamRepository repository, RenderService rs) {
        this.repository = repository;
        this.rs = rs;
    }

    static class RenderResponse {
        public SceneParam SceneParam;
        public String user_id;

        RenderResponse(SceneParam SceneParam, String userId) {
            this.SceneParam = SceneParam;
            this.user_id = userId;
        }
    }

    @PostMapping("/render")
    public RenderResponse renderResponse(@AuthenticationPrincipal OAuth2User principal,
            @RequestBody SceneParam newSceneParam) {
        SceneParam sp = repository.save(newSceneParam);
        String user_id = principal.getAttribute("sub");
        rs.RenderProcess(sp, user_id);
        return new RenderResponse(sp, user_id);
    }

    // SceneParam newSceneParam(@RequestBody SceneParam newSceneParam) {
    // rs.RenderProcess(newSceneParam);
    // return repository.save(newSceneParam);
    // }
}
