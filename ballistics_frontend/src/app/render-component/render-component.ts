import { Component, Input, signal } from '@angular/core';
import { inject } from '@angular/core';
import { RenderService } from '../render-service';

@Component({
  selector: 'app-render-component',
  imports: [],
  templateUrl: './render-component.html',
  styleUrl: './render-component.css',
})
export class renderComponent {
  @Input() vid_src = "";
  @Input() vid_id = "";
  readonly failed = signal(false);
  renderService = inject(RenderService);
  onVideoError(): void {
    this.failed.set(true);
  }
  
  onVideoReady(): void {
    this.renderService.videoLoaded.set(true);
  }

  deleteVideo(id: string) {
    this.renderService.deleteJob(id)
  }
}
