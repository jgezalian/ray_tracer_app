import { Component } from '@angular/core';
import { InputParams } from './input-params';
import { FormsModule } from '@angular/forms';
import { NgForm } from '@angular/forms';
import { Injectable, inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-input-form-component',
  imports: [FormsModule],
  templateUrl: './input-form-component.html',
  styleUrl: './input-form-component.css',
})
@Injectable({ providedIn: 'root' })
export class InputFormComponent {
  private http = inject(HttpClient);
  submitted = false;

  model = new InputParams(0, 0);

  onSubmit(form: NgForm) {
    if (form.invalid) {
      form.control.markAllAsTouched();
      return;
    }

    const userParams = new InputParams(this.model.v_x, this.model.v_y);
    this.http
      .post<string>('api/render', userParams, {
        mode: 'cors',
        withCredentials: true
      })
      .subscribe((response) => {
        console.log(response);
        
      });
    this.submitted = true;
  }
}
