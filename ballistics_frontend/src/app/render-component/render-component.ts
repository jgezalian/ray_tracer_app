import { Component, Input, signal } from '@angular/core';

@Component({
  selector: 'app-render-component',
  imports: [],
  templateUrl: './render-component.html',
  styleUrl: './render-component.css',
})
export class renderComponent {
  @Input() vid_src = "";
  readonly failed = signal(false);

  onVideoError(): void {
    this.failed.set(true);
  }
}
