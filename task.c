#include "task.h"
#include "addr.h"
#include "context.h"
#include "malloc.h"
#include "uart.h"

typedef struct {
  task_t **q;
  int new_idx;
  int rr_idx;
} task_queue;

task_queue *tq = 0;

void task_create(void (*f)()) {
  if (tq == 0) {
    tq = simple_malloc(sizeof(task_queue));
    tq->rr_idx = 0;
    tq->q = simple_malloc(sizeof(task_t *) * 10);
    tq->new_idx = 0;
  }

  if (tq->new_idx == 10) {
    uart_puts("too many threads");
    return;
  }

  task_t *t = simple_malloc(sizeof(task_t));

  t->regs.lr = (unsigned long)f;
  t->regs.fp = KERNEL_TASK_ADDR + tq->new_idx * 10000;
  t->regs.sp = t->regs.fp;
  t->valid = 1;
  tq->q[tq->new_idx] = t;
  tq->new_idx++;
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
