package com.jgezalian.ballistics_backend.controllers;

import java.util.List;
import com.jgezalian.ballistics_backend.entity.SceneParam;
import com.jgezalian.ballistics_backend.repository.SceneParamRepository;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class SceneParamController {
	private final SceneParamRepository repository;

	SceneParamController(SceneParamRepository repository) {
		this.repository = repository;
	}

	@GetMapping("/scene_params")
	List<SceneParam> all() {
		return repository.findAll();
	}


	@DeleteMapping("/scene_params/{id}")
	void deleteSceneParam(@PathVariable Integer id) {
		repository.deleteById(id);
	}
}
