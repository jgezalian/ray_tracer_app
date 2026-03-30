import { Component, signal } from '@angular/core';
import { OauthComponent } from "./oauth-component/oauth-component";


@Component({
  selector: 'app-root',
  imports: [OauthComponent],
  templateUrl: './app.html',
  styleUrl: './app.css'
})
export class App {
  protected readonly title = signal('ballistics_frontend');
}
