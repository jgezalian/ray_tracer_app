import { Injectable, inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { environment } from '../environments/environment';
import { signal, computed } from '@angular/core';

export interface userDto {
  name: string;
}

@Injectable({
  providedIn: 'root',
})
export class AuthService {
  private http = inject(HttpClient);
  readonly user = signal<userDto | null>(null);
  readonly loading = signal(false);
  readonly error = signal<string | null>(null);

  readonly isAuthenticated = computed(() => this.user() != null);
  readonly loginUrl = environment.loginUrl;

  getCurrentUser(): void {
    this.loading.set(true);
    this.error.set(null);
    this.http.get<userDto>(environment.baseApiUrl + '/user', { withCredentials: true }).subscribe({
      next: (user) => {
        this.user.set(user);
        this.loading.set(false);
        this.error.set(null);
      },
      error: (err) => {
        if (err.status === 401 || err.status === 403) {
          this.user.set(null);
          this.error.set(null);
        } else this.error.set('Failed to load user: ' + err.message);
        this.loading.set(false);
      },
    });
  }

  logout(): void {
    this.http.post(environment.baseApiUrl + '/logout', {}, { responseType: 'text', withCredentials: true }).subscribe({
      next: (user) => {
        this.user.set(null);
      },
      error: (err) => {
        this.error.set('Failed to log out: ' + err.message);
      }
    });
  }
}
