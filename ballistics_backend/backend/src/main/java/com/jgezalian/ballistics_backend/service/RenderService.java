package com.jgezalian.ballistics_backend.service;

import com.jgezalian.ballistics_backend.exception.RenderException;
import java.lang.ProcessBuilder;
import java.lang.ProcessBuilder.Redirect;
import java.io.File;
import java.io.IOException;
import org.springframework.stereotype.Service;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.nio.file.Files;
import org.springframework.beans.factory.annotation.Value;

@Service
public class RenderService {

    private final Path tmpDirPath;

    public RenderService(VideoStorageService vs,
            @Value("${app.tmp.dir}") String tmpDirPathString) {
        this.tmpDirPath = Paths.get(tmpDirPathString);
    }

    public void dirCheck(Path tmpDirPath) {
        try {
            Files.createDirectories(tmpDirPath);
            return;
        }

        catch (IOException e) {
            throw new RenderException("Failed to create directory" + tmpDirPath, e);
        }

    }

    public Path renderProcess(float vX, float vY, String userId, Long id) {
        Long jobId = id;
        String jobIdString = jobId.toString();
        Path videoPath = Paths.get("");
        Path jobDir = tmpDirPath.resolve(jobIdString);
        dirCheck(jobDir);
        String v_x = String.valueOf(vX);
        String v_y = String.valueOf(vY);

        try {
            Path exe = Paths.get("src/main/c/physics_test").toAbsolutePath();
            ProcessBuilder pb = new ProcessBuilder(exe.toString(), v_x, v_y);
            pb.directory(jobDir.toFile());

            File log = jobDir.resolve("render_rgb.log").toFile();
            pb.redirectErrorStream(true);
            pb.redirectOutput(Redirect.appendTo(log));

            Process p = pb.start();

            assert pb.redirectInput() == Redirect.PIPE;
            assert pb.redirectOutput().file() == log;
            assert p.getInputStream().read() == -1;

            int exitCode = p.waitFor();
            if (exitCode != 0) {
                throw new RenderException("RGB render process exited with code: " + exitCode);
            }
            System.out.println("\nRGB render process exited with code: " + exitCode);

        } catch (IOException e) {
            throw new RenderException("Failed to run rgb renderer", e);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new RenderException("RGB renderer was interrupted", e);
        }

        Path renderRgb = jobDir.resolve("render.rgb");
        if (!Files.exists(renderRgb)) {
            throw new RenderException("render.rgb file not found");
        }

        try {
            ProcessBuilder pb = new ProcessBuilder(
                    "ffmpeg", "-f", "rawvideo", "-pix_fmt", "rgb24", "-video_size", "1280x720",
                    "-framerate", "120",
                    "-i", "render.rgb", "-c:v", "libx264",
                    "-crf", "20",
                    "-pix_fmt", "yuv420p", "render.mp4");

            pb.directory(jobDir.toFile());

            File log = jobDir.resolve("render_video.log").toFile();
            pb.redirectErrorStream(true);
            pb.redirectOutput(Redirect.appendTo(log));

            Process p = pb.start();

            assert pb.redirectInput() == Redirect.PIPE;
            assert pb.redirectOutput().file() == log;
            assert p.getInputStream().read() == -1;

            int exitCode = p.waitFor();
            videoPath = jobDir.resolve("render.mp4");

            if (exitCode != 0) {
                throw new RenderException("Video render process exited with code: " + exitCode);
            }
            if (!Files.exists(videoPath)) {
                throw new RenderException("render.mp4 file not found");
            }
            Files.delete(renderRgb);
            System.out.println("\nVideo render process exited with code: " + exitCode);
        } catch (IOException e) {
            throw new RenderException("Failed to run video renderer", e);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new RenderException("Video renderer was interrupted", e);
        }

        return videoPath;

    }

}
