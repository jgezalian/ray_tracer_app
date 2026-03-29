package com.jgezalian.ballistics_backend.service;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import org.springframework.stereotype.Service;

@Service
public class JobExecutorService {

    private final Executor executor = Executors.newFixedThreadPool(2);

    public JobExecutorService() {
    }

    public void submit(Runnable job) {
        executor.execute(job);
    }
}
