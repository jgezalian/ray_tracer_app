package com.jgezalian.ballistics_backend.dto;

public class CreateJobRequest {
    private Double vX;
    private Double vY;

    public CreateJobRequest() {}

    public CreateJobRequest(Double vX, Double vY) {
        this.vX = vX;
        this.vY = vY;
    }

    public Double getVx() {
        return vX;
    }

    public void setVx(Double vX) {
        this.vX = vX;
    }

    public Double getVy() {
        return this.vY;
    }

    public void setVy(Double vY) {
        this.vY = vY;
    }

}
