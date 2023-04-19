#pragma once

typedef struct trapframe {
  unsigned long x[31]; // general register from x0 ~ x30
  unsigned long sp_el0;
  unsigned long elr_el1;
  unsigned long spsr_el1;
} trapframe_t;

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
  int tid;
  long user_sp;
  trapframe_t *tf;
} task_t;

void task_create(void (*f)());
void schedule();
void task_end();
int task_getid();
task_t *get_current();
int task_clone_current(trapframe_t *tf);
void shell_task();
