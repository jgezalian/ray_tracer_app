import { Component, signal } from '@angular/core';
import { InputParams } from './input-params';
import { FormsModule } from '@angular/forms';
import { NgForm } from '@angular/forms';
import { inject } from '@angular/core';
import { EventEmitter, Output } from '@angular/core';
import { jobRequestDto, RenderService, jobResponseDto } from '../render-service';

@Component({
  selector: 'app-input-form-component',
  imports: [FormsModule],
  templateUrl: './input-form-component.html',
  styleUrl: './input-form-component.css',
})
export class InputFormComponent {
  renderService = inject(RenderService);
  
  submitted = false;
  model = new InputParams(0, 0, 0);
  readonly error = signal<string | null>(null);
  @Output() rendered = new EventEmitter<void>();

  onSubmit(form: NgForm) {
    if (form.invalid) {
      form.control.markAllAsTouched();
      return;
    }
    this.error.set(null);

    const body: jobRequestDto = {
      vX: this.model.vX,
      vY: this.model.vY,
      vZ: this.model.vZ
    };
    this.renderService.createJob(body);
    this.renderService.videoLoaded.set(false);
  }
}
