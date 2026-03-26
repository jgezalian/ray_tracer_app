import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ListRendersComponent } from './list-renders-component';

describe('ListRendersComponent', () => {
  let component: ListRendersComponent;
  let fixture: ComponentFixture<ListRendersComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ListRendersComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ListRendersComponent);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
