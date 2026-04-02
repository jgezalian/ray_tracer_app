import { Injectable, inject, signal } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from '../environments/environment';
import { interval } from 'rxjs';

export interface jobRequestDto {
  vX: number;
  vY: number;
  vZ: number;
}

export interface jobResponseDto {
  id: string;
  jobStatus: 'QUEUED' | 'RUNNING' | 'COMPLETED' | 'FAILED';
  errorMsg: string | null;
}

export interface VideoDto {
  id: string;
  url: string;
}

@Injectable({
  providedIn: 'root',
})
export class RenderService {
  private http = inject(HttpClient);
  readonly videoUrls = signal<VideoDto[]>([]);
  readonly listLoading = signal(false);
  readonly listError = signal<string | null>(null);

  readonly currentJob = signal<jobResponseDto | null>(null);
  readonly jobRequestLoading = signal(false);
  readonly jobRequestError = signal<string | null>(null);
  readonly jobResponseLoading = signal(false);
  readonly jobResponseError = signal<string | null>(null);
  readonly jobExecutionError = signal<string | null>(null);
  readonly jobDeleteError = signal<string | null>(null);
  readonly jobSubmitted = signal(false);
  readonly getRenderLoading = signal(false);
  readonly getRenderError = signal<string | null>(null);
  videoLoaded = signal(true);
  private intervalId = signal<number | undefined>(undefined);
  //private pollSub: Subscription | null = null;

  createJob(body: jobRequestDto): void {
    this.jobRequestError.set(null);
    this.jobRequestLoading.set(true);
    this.jobSubmitted.set(false);
    this.http
      .post<jobResponseDto>(environment.baseApiUrl + '/jobs', body, {
        withCredentials: true,
      })
      .subscribe({
        next: (job) => {
          this.currentJob.set(job);
          this.jobSubmitted.set(true);
          this.jobRequestLoading.set(false);
          this.pollJob(job.id);
        },
        error: (err) => {
          this.jobRequestError.set('Failed to send job request: ' + err.message);
          this.jobRequestLoading.set(false);
        },
      });
  }

  getRender(id: string): void {
    this.videoLoaded.set(false);
    this.getRenderLoading.set(true);
    this.http
      .get<VideoDto>(`${environment.baseApiUrl}/get_render/${id}`, { withCredentials: true })
      .subscribe({
        next: (video) => {
          this.videoUrls.update((urls) => {
            const withoutOld = urls.filter((v) => v.id !== video.id);
            return [video, ...withoutOld];
          });
          this.getRenderLoading.set(false);
        },
        error: (err) => {
          this.getRenderError.set(err.message);
          this.getRenderLoading.set(false);
        },
      });
  }

  getJob(id: string): void {
    this.jobResponseError.set(null);
    this.http
      .get<jobResponseDto>(`${environment.baseApiUrl}/jobs/${id}`, { withCredentials: true })
      .subscribe({
        next: (job) => {
          this.jobResponseLoading.set(false);
          if (job.jobStatus == 'COMPLETED') {
            this.currentJob.set(job);
            clearInterval(this.intervalId());
            //change this to get single video rather than refresh entire list
            this.getRender(job.id);
          }
          if (job.jobStatus == 'FAILED') {
            this.currentJob.set(job);
            clearInterval(this.intervalId());
            this.jobExecutionError.set('job failed to complete: ' + job.errorMsg);
          }

          if (job.jobStatus == 'QUEUED' || job.jobStatus == 'RUNNING') {
            this.currentJob.set(job);
          }
        },
        error: (err) => {
          this.jobResponseLoading.set(false);
          clearInterval(this.intervalId());
          this.jobResponseError.set(`could not get job: ${id} : ${err.message}`);
        },
      });
  }

  pollJob(id: string): void {
    clearInterval(this.intervalId());
    this.intervalId.set(undefined);
    this.jobResponseLoading.set(true);
    this.intervalId.set(window.setInterval(() => this.getJob(id), 1000));
  }

  listRenders(): void {
    this.videoUrls.set([]);
    this.listError.set(null);
    this.listLoading.set(true);
    this.http
      .get<VideoDto[]>(environment.baseApiUrl + '/list_renders', { withCredentials: true })
      .subscribe({
        next: (response) => {
          this.videoUrls.set(response);
          this.listLoading.set(false);
          this.listError.set(null);
        },
        error: (err) => {
          this.listError.set(err.message);
          this.listLoading.set(false);
        },
      });
  }

  deleteJob(id: string): void {
    this.http.delete(`${environment.baseApiUrl}/jobs/${id}`, { withCredentials: true }).subscribe({
      next: (response) => {
        this.videoUrls.update((urls) => {
          const withoutDeleted = urls.filter((v) => v.id !== id);
          return withoutDeleted;
        });
      },
      error: (err) => {
        this.jobDeleteError.set(`could not delete job: ${id} : ${err.message}`);
      },
    });
  }

  deleteVideo() {}

  // angular polling
  //   pollJob(jobId: string): void {
  //   this.pollSub?.unsubscribe();

  //   this.pollSub = timer(0, 2000)
  //     .pipe(
  //       switchMap(() =>
  //         this.http.get<jobResponseDto>(`${environment.baseApiUrl}/jobs/${jobId}`, {
  //           withCredentials: true,
  //         })
  //       ),
  //       tap((job) => {
  //         this.currentJob.set(job);
  //       }),
  //       takeWhile(
  //         (job) => job.status === 'QUEUED' || job.status === 'RUNNING',
  //         true
  //       )
  //     )
  //     .subscribe({
  //       next: (job) => {
  //         if (job.status === 'COMPLETED') {
  //           this.listRenders();
  //         }

  //         if (job.status === 'FAILED') {
  //           this.jobRequestError.set(job.error ?? 'Job failed');
  //         }
  //       },
  //       error: (err) => {
  //         this.jobRequestError.set(`Polling failed: ${err.message}`);
  //       },
  //     });
  // }
}
