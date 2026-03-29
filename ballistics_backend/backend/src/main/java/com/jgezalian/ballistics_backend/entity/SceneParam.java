package com.jgezalian.ballistics_backend.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.Table;

@Entity
@Table(name = "scene_params")
public class SceneParam {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    private float vX;
    private float vY;

    public SceneParam() {

    }

    public SceneParam(float vX, float vY) {
        this.vX = vX;
        this.vY = vY;
    }

    public float[] getVelocities() {
        float[] vs = { this.vX, this.vY };
        return vs;
    }

    public float getVx() {
        return vX;
    }

    public float getVy() {
        return vY;
    }

    public void setVx(float _vX) {
        this.vX = _vX;
    }

    public void setVy(float _vY) {
        this.vY = _vY;
    }

}
