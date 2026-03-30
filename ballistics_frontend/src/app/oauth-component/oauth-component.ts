import { Component } from '@angular/core';
import { inject } from '@angular/core';
import { InputFormComponent } from '../input-form-component/input-form-component';
import { ListRendersComponent } from '../list-renders-component/list-renders-component';
import { AuthService } from '../auth-service';

@Component({
  selector: 'app-oauth-component',
  imports: [InputFormComponent, ListRendersComponent],
  templateUrl: './oauth-component.html',
  styleUrl: './oauth-component.css',
})
export class OauthComponent {
  authService = inject(AuthService);

  ngOnInit() {
    this.authService.getCurrentUser();
  }

  logout(): void {
    this.authService.logout();
  }
}
