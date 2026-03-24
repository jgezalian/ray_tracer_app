import { Component, signal } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { videoComponent } from './video_component/video-component';
import { InputFormComponent } from "./input-form-component/input-form-component";
import { OauthComponent } from "./oauth-component/oauth-component";

@Component({
  selector: 'app-root',
  imports: [RouterOutlet, videoComponent, InputFormComponent, OauthComponent],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  protected readonly title = signal('ballistics_frontend');
}
