package com.jgezalian.ballistics_backend.dto;

public class CreateJobRequest {
    private Double vX;
    private Double vY;
    private Double vZ;

    public CreateJobRequest() {
    }

    public CreateJobRequest(Double vX, Double vY, double vZ) {
        this.vX = vX;
        this.vY = vY;
        this.vZ = vZ;
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

    public Double getVz() {
        return this.vZ;
    }

    public void setVz(Double vZ) {
        this.vZ = vZ;
    }

}
