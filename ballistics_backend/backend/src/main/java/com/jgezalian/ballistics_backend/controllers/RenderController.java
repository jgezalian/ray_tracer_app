package com.jgezalian.ballistics_backend.controllers;

import java.util.List;
import com.jgezalian.ballistics_backend.entity.SceneParam;
import com.jgezalian.ballistics_backend.repository.SceneParamRepository;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import com.jgezalian.ballistics_backend.service.RenderService;

@RestController
@CrossOrigin(origins = "http://localhost:4200", allowCredentials = "true")
public class RenderController {
    private final SceneParamRepository repository;
    private final RenderService rs;

    RenderController(SceneParamRepository repository, RenderService rs) {
		this.repository = repository;
        this.rs = rs;
	}


    @PostMapping("/render")
    SceneParam newSceneParam(@RequestBody SceneParam newSceneParam) {
        rs.RenderProcess(newSceneParam);
        return repository.save(newSceneParam);
    }
}
