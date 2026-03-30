import { Component, signal } from '@angular/core';
import { inject } from '@angular/core';
import { renderComponent } from "../render-component/render-component";
import { RenderService } from '../render-service';


@Component({
  selector: 'app-list-renders-component',
  imports: [renderComponent],
  templateUrl: './list-renders-component.html',
  styleUrl: './list-renders-component.css',
})
export class ListRendersComponent {
  renderService = inject(RenderService);

  listRenders(): void {
    this.renderService.listRenders();
  }
  ngOnInit() {
    this.listRenders();
  }
  
}
