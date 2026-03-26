import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-render-component',
  imports: [],
  templateUrl: './render-component.html',
  styleUrl: './render-component.css',
})
export class renderComponent {
  @Input() vid_src = "";
}
