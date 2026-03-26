import { Component, signal } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Injectable, inject } from '@angular/core';
import { renderComponent } from "../render-component/render-component";


@Component({
  selector: 'app-list-renders-component',
  imports: [renderComponent],
  templateUrl: './list-renders-component.html',
  styleUrl: './list-renders-component.css',
})
export class ListRendersComponent {
  private http = inject(HttpClient);
  videoUrls = signal<string[]>([]);
  listRenders() {
    return this.http
      .get<string[]>('/api/list_renders', { withCredentials: true })
      .subscribe((response) => {
        this.videoUrls.set(response);
      });
  }


  ngOnInit() {
    this.listRenders();
  }
  
}
