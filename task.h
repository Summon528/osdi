#pragma once

typedef struct {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp; // x29
  unsigned long lr; // x30
  unsigned long sp;
} registers_t;

typedef struct {
  registers_t regs;
  int valid;
} task_t;

void task_create(void (*f)());
void schedule();
void shell_task();
