package com.jgezalian.ballistics_backend.service;

import java.lang.ProcessBuilder;
import java.lang.ProcessBuilder.Redirect;
import java.io.File;
import java.io.IOException;
import com.jgezalian.ballistics_backend.entity.SceneParam;
import software.amazon.awssdk.services.s3.endpoints.internal.Value.Bool;

import org.springframework.stereotype.Service;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import org.springframework.beans.factory.annotation.Value;
import java.util.UUID;

@Service
public class RenderService {

    private final VideoStorageService vs;
    private final Path tmp_dir_path;
    private final Path tmp_dir_path_rgb;
    private final Path tmp_dir_path_renders;

    public RenderService(VideoStorageService vs,
            @Value("${app.tmp.dir}") String tmp_dir_path_string,
            @Value("${app.tmp.dir.rgb}") String tmp_dir_path_rgb_string,
            @Value("${app.tmp.dir.renders}") String tmp_dir_path_renders_string) {
        this.vs = vs;
        this.tmp_dir_path = Paths.get(tmp_dir_path_string);
        this.tmp_dir_path_rgb = Paths.get(tmp_dir_path_rgb_string);
        this.tmp_dir_path_renders = Paths.get(tmp_dir_path_renders_string);
    }

    public void dir_check(Path tmp_dir_path) {
        try {
            if (!(Files.exists(tmp_dir_path) && Files.isDirectory(tmp_dir_path))) {
                Files.createDirectory(tmp_dir_path);
                Files.createDirectory(tmp_dir_path_rgb);
                Files.createDirectory(tmp_dir_path_renders);
            } else
                return;
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    public void RenderProcess(SceneParam sp) {

        dir_check(tmp_dir_path);
        float[] v = sp.getVelocities();
        String v_x = String.valueOf(v[0]);
        String v_y = String.valueOf(v[1]);

        try {
            Path exe = Paths.get("src/main/c/physics_test").toAbsolutePath();
            ProcessBuilder pb = new ProcessBuilder(exe.toString(), v_x, v_y);
            pb.directory(tmp_dir_path_rgb.toFile());
            File log = new File("log");
            pb.redirectErrorStream(true);
            pb.redirectOutput(Redirect.appendTo(log));
            Process p = pb.start();
            assert pb.redirectInput() == Redirect.PIPE;
            assert pb.redirectOutput().file() == log;
            assert p.getInputStream().read() == -1;
            int exitCode = p.waitFor();
            System.out.println("\nProcess exited with code: " + exitCode);

        } catch (IOException | InterruptedException e) {
            System.err.println("Caught IOException: " + e.getMessage());
        }

        Path render_rgb = Paths.get(".tmp/rgb/render.rgb");
        UUID job_id = UUID.randomUUID();
        String job_id_string = job_id.toString();

        try {
            ProcessBuilder pb = new ProcessBuilder(
                    "ffmpeg", "-f", "rawvideo", "-pix_fmt", "rgb24", "-video_size", "1280x720",
                    "-framerate", "120",
                    "-i", "../rgb/render.rgb", job_id_string + ".mp4");

            pb.directory(tmp_dir_path_renders.toFile());
            File log = new File("log1");
            pb.redirectErrorStream(true);
            pb.redirectOutput(Redirect.appendTo(log));
            Process p = pb.start();
            assert pb.redirectInput() == Redirect.PIPE;
            assert pb.redirectOutput().file() == log;
            assert p.getInputStream().read() == -1;
            int exitCode = p.waitFor();
            Path video = Paths.get(".tmp/renders/" + job_id_string + ".mp4");

            if (exitCode == 0 && Files.exists(video)) {
                Files.delete(render_rgb);
            }
            System.out.println("\nProcess exited with code: " + exitCode);

        } catch (IOException | InterruptedException e) {
            System.err.println("Caught IOException: " + e.getMessage());
        }

        vs.storeVideo(job_id_string);

    }

}
