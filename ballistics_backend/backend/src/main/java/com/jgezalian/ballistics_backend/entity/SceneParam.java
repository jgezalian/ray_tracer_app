package com.jgezalian.ballistics_backend.entity;

import java.util.Objects;

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
    private float v_x;
    private float v_y;

    public SceneParam() {

    }

    public SceneParam(float v_x, float v_y) {
        this.v_x = v_x;
        this.v_y = v_y;
    }

    public float[] getVelocities() {
        float[] vs = {this.v_x, this.v_y};
        return vs;
    }

    public float getV_x() {
        return v_x;
    }

    public float getV_y() {
        return v_y;
    }

    public void setV_x(float _v_x) {
        this.v_x = _v_x;
    }

    public void setV_y(float _v_y) {
        this.v_y = _v_y;
    }

}
