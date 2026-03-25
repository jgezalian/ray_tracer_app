import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Injectable, inject } from '@angular/core';
import { User } from './user';
import { InputFormComponent } from '../input-form-component/input-form-component';
import { signal } from '@angular/core';

@Component({
  selector: 'app-oauth-component',
  imports: [InputFormComponent],
  templateUrl: './oauth-component.html',
  styleUrl: './oauth-component.css',
})
@Injectable({ providedIn: 'root' })
export class OauthComponent {
  private http = inject(HttpClient);

  private user = new User();
  private auth = signal<boolean | null>(null);

  getAuth() {
    return this.auth();
  }

  getUser() {
    return this.user;
  }

  getUserData() {
    type user_response = { name: string };
    return this.http.get<user_response>('api/user', { withCredentials: true });
  }

  ngOnInit() {
    this.getUserData().subscribe({
      next: (user) => {
        console.log(user);
        this.user.setName(user.name);
        this.auth.set(true);
      },
      error: (err) => {
        if (err.status === 401 || err.status === 403) {
          this.auth.set(false);
        }
      },
    });
  }


}
