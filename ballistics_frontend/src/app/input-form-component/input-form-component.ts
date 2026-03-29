import { Component } from '@angular/core';
import { InputParams } from './input-params';
import { FormsModule } from '@angular/forms';
import { NgForm } from '@angular/forms';
import { Injectable, inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { EventEmitter, Output } from '@angular/core';

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
  @Output() rendered = new EventEmitter<void>();

  onSubmit(form: NgForm) {
    if (form.invalid) {
      form.control.markAllAsTouched();
      return;
    }

    const userParams = new InputParams(this.model.vX, this.model.vY);
    this.http
      .post<string>('api/jobs', userParams, {
        mode: 'cors',
        withCredentials: true,
      })
      .subscribe(() => this.rendered.emit());
    this.submitted = true;
  }
}
