#include "task.h"
#include "addr.h"
#include "context.h"
#include "exec.h"
#include "malloc.h"
#include "uart.h"
#include "utils.h"

typedef struct {
  task_t **q;
  int new_idx;
  int rr_idx;
} task_queue;

task_queue *tq = 0;
int tid = 0;

void task_create(void (*f)()) {
  if (tq == 0) {
    tid = 0;
    tq = simple_malloc(sizeof(task_queue));
    tq->rr_idx = 0;
    tq->q = simple_malloc(sizeof(task_t *) * 100);
    tq->new_idx = 0;
  }

  if (tq->new_idx == 100) {
    uart_puts("too many threads");
    return;
  }

  task_t *t = simple_malloc(sizeof(task_t));

  t->user_sp = 0;
  t->regs.lr = (unsigned long)f;
  t->tf = simple_malloc(2048) + 1024;
  t->regs.fp = (long)t->tf;
  t->regs.sp = t->regs.fp;
  t->valid = 1;
  t->tid = tid++;
  tq->q[tq->new_idx] = t;
  tq->new_idx++;
}

void temp_load_all() {
  asm volatile("ldp x21, x22, [sp, #16 * 16]\r\n"
               "msr spsr_el1, x22\r\n"
               "msr elr_el1, x21\r\n"
               "ldp x30, x21, [sp, #16 * 15]\r\n"
               "msr sp_el0, x21\r\n"
               "ldp x28, x29, [sp, #16 * 14]\r\n"
               "ldp x26, x27, [sp, #16 * 13]\r\n"
               "ldp x24, x25, [sp, #16 * 12]\r\n"
               "ldp x22, x23, [sp, #16 * 11]\r\n"
               "ldp x20, x21, [sp, #16 * 10]\r\n"
               "ldp x18, x19, [sp, #16 * 9]\r\n"
               "ldp x16, x17, [sp, #16 * 8]\r\n"
               "ldp x14, x15, [sp, #16 * 7]\r\n"
               "ldp x12, x13, [sp, #16 * 6]\r\n"
               "ldp x10, x11, [sp, #16 * 5]\r\n"
               "ldp x8, x9, [sp, #16 * 4]\r\n"
               "ldp x6, x7, [sp, #16 * 3]\r\n"
               "ldp x4, x5, [sp, #16 * 2]\r\n"
               "ldp x2, x3, [sp, #16 * 1]\r\n"
               "ldp x0, x1, [sp, #16 * 0]\r\n"
               "add sp, sp, #272\r\n"
               "eret\r\n");
}

int task_clone_current(trapframe_t *tf) {
  if (tq->new_idx == 100) {
    uart_puts("too many threads");
    return 0;
  }
  task_t *t1 = get_current();
  task_t *t2 = simple_malloc(sizeof(task_t));
  memcpy(t2, t1, sizeof(task_t));
  t2->user_sp = USER_PROG_STACK_ADDR + tid * 10000;
  memcpy((void *)t2->user_sp - 10000, (void *)t1->user_sp - 10000, 10000);
  t2->tf = simple_malloc(2048) + 1024;
  memcpy(t2->tf, tf, sizeof(trapframe_t));
  t2->tf->sp_el0 = t2->user_sp + (tf->sp_el0 - t1->user_sp);
  t2->tf->x[0] = 0;
  t2->regs.sp = (long)t2->tf;
  t2->regs.lr = (long)&temp_load_all;
  t2->tid = tid++;
  tq->q[tq->new_idx] = t2;
  tq->new_idx++;
  return t2->tid;
}

void schedule() {
  int prev_idx = tq->rr_idx;
  while (1) {
    tq->rr_idx = (tq->rr_idx + 1) % tq->new_idx;
    if (tq->q[tq->rr_idx]->valid) {
      break;
    }
  }
  switch_to(tq->q[prev_idx], tq->q[tq->rr_idx]);
}

void task_end() {
  task_t *t = (task_t *)get_current();
  t->valid = 0;
  schedule();
}

int task_getid() {
  task_t *t = (task_t *)get_current();
  return t->tid;
}

void gogo() {
  while (1) {
    uart_puts("1");
  }
  task_end();
}
void gogo2() {
  while (1) {
    uart_puts("2");
  }
  task_end();
}

void gogo3() {
  while (1) {
    uart_puts("3");
  }
  task_end();
}

void shell_task() {
  task_create(gogo);
  task_create(gogo2);
  task_create(gogo3);
}
