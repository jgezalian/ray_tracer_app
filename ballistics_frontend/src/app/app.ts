import { Component, signal } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { renderComponent } from './render-component/render-component';
import { InputFormComponent } from "./input-form-component/input-form-component";
import { OauthComponent } from "./oauth-component/oauth-component";
import { ListRendersComponent } from './list-renders-component/list-renders-component';

@Component({
  selector: 'app-root',
  imports: [RouterOutlet, renderComponent, InputFormComponent, OauthComponent, ListRendersComponent],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  protected readonly title = signal('ballistics_frontend');
}
