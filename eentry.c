#include "eentry.h"
#include "task.h"
#include "uart.h"

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
  asm volatile("eret");
}

void irq_entry() {
  unsigned long freq;
  asm volatile("mrs x1, cntfrq_el0\n\t"
               "mov %0, x1\n\t"
               : "=r"(freq)
               :
               : "x1");

  unsigned long ct;
  asm volatile("mrs x1, cntpct_el0\n\t"
               "mov %0, x1\n\t"
               : "=r"(ct)
               :
               : "x1");

  uart_puts("Uptime: ");
  uart_hex(ct / freq);
  uart_puts("\n");
  uart_puts("\n");

  unsigned long freq2 = freq * 2;
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
}

void schedule_entry() {
  unsigned long freq;
  asm volatile("mrs x1, cntfrq_el0\n\t"
               "mov %0, x1\n\t"
               : "=r"(freq)
               :
               : "x1");

  unsigned long freq2 = freq;
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
