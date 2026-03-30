import { Injectable, inject, signal } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from '../environments/environment';
import { interval } from 'rxjs';

export interface jobRequestDto {
  vX: number;
  vY: number;
}

export interface jobResponseDto {
  id: string;
  jobStatus: 'QUEUED' | 'RUNNING' | 'COMPLETED' | 'FAILED';
  error: string | null;
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
  readonly jobSubmitted = signal(false);
  private intervalId = signal<number | undefined>(undefined);
  //private pollSub: Subscription | null = null;

  createJob(body: jobRequestDto): jobResponseDto | null {
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
          console.log('Created job: ' + 'id: ' + job.id + ' ' + 'status: ' + job.jobStatus);
        },
        error: (err) => {
          this.jobRequestError.set('Failed to send job request: ' + err.message);
          this.jobRequestLoading.set(false);
        },
      });

    return this.currentJob();
  }

  getJob(id: string): void {
    console.log('polling...');
    this.http.get<jobResponseDto>(`${environment.baseApiUrl}/jobs/${id}`,  { withCredentials: true }).subscribe({
      next: (job) => {
        if (job.jobStatus == 'COMPLETED' || job.jobStatus == 'FAILED') {
          console.log('job done, stopping polling...');
          clearInterval(this.intervalId());
          //change this to get single video rather than refresh entire list 
          this.listRenders();
        }
      },
    });
  }

  pollJob(id: string): void {
    this.intervalId.set(undefined);
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
          this.listLoading.set(false);
        },
      });
  }

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
