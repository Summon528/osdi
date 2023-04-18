#include "eentry.h"
#include "task.h"
#include "uart.h"
#include "user/sys.h"

typedef struct trapframe {
  unsigned long x[31]; // general register from x0 ~ x30
  unsigned long sp_el0;
  unsigned long elr_el1;
  unsigned long spsr_el1;
} trapframe_t;

void exception_entry() {
  unsigned long x;
  asm volatile("mrs x1, spsr_el1\n\t"
               "mov %0, x1\n\t"
               : "=r"(x)
               :
               : "x1");
  uart_puts("SPSR_EL1: ");
  uart_hex(x);
  uart_puts("\n");

  asm volatile("mrs x1, elr_el1\n\t"
               "mov %0, x1\n\t"
               : "=r"(x)
               :
               : "x1");
  uart_puts("ELR_EL1: ");
  uart_hex(x);
  uart_puts("\n");

  asm volatile("mrs x1, esr_el1\n\t"
               "mov %0, x1\n\t"
               : "=r"(x)
               :
               : "x1");
  uart_puts("ESR_EL1: ");
  uart_hex(x);
  uart_puts("\n");
}

void syscall_entry(trapframe_t *tf) {
  switch (tf->x[0]) {
  case SYS_EXIT:
    task_end();
    break;
  case SYS_GETPID:
    tf->x[0] = task_getid();
    break;
  case SYS_UART_WRITE:
    uart_puts((char *)tf->x[1]);
    break;
  }
}

void schedule_entry() {
  unsigned long freq;
  asm volatile("mrs x1, cntfrq_el0\n\t"
               "mov %0, x1\n\t"
               : "=r"(freq)
               :
               : "x1");

  unsigned long freq2 = freq / 100;
  asm volatile("mov x0, 1\n\t"
               "msr cntp_ctl_el0, x0\n\t"
               "mov x0, %0\n\t"
               "msr cntp_tval_el0, x0\n\t"
               "mov x0, 2\n\t"
               "ldr x1, =0x40000040\n\t"
               "str w0, [x1]\n\t"
               :
               : "r"(freq2)
               : "x0", "x1");

  schedule();
}
