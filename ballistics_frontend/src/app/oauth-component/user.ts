import { signal } from '@angular/core';

export class User {
  constructor(private name = signal<string | null>(null)) {}
  getName(): string | null {
    return this.name();
  }
  setName(name: string | null) {
    this.name.set(name);
  }
}
