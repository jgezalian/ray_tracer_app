CREATE TABLE IF NOT EXISTS jobs (
    id serial PRIMARY KEY,
    user_id text NOT NULL,
    v_x double precision NOT NULL,
    v_y double precision NOT NULL,
    job_status text NOT NULL,
    created_at timestamp NOT NULL,
    started_at timestamp,
    finished_at timestamp,
    s3_key text,
    error_msg text
);