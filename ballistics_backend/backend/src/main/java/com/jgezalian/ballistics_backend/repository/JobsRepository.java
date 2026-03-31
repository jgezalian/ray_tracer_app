package com.jgezalian.ballistics_backend.repository;
import org.springframework.data.jpa.repository.JpaRepository;
import com.jgezalian.ballistics_backend.entity.Job;
import java.util.List;
import java.util.Optional;

public interface JobsRepository extends JpaRepository<Job, Long> {
    List<Job> findByUserIdOrderByCreatedAtDesc(String userId);
    List<Job> findByUserIdAndJobStatusOrderByCreatedAtDesc(String userId, Job.JobStatus status);
    Optional<Job> findByUserIdAndId(String userId, Long id);
    Optional<Job> findById(Long id);
    void deleteByUserIdAndId(String userId, Long id);
    Optional<Job> findByUserIdAndIdAndJobStatus(String userId, Long id, Job.JobStatus status);
}
