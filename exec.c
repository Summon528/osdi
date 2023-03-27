#include "addr.h"
#include "cpio.h"
#include "uart.h"

void shell_exec() {
  void *addr = (void *)USER_PROG_ADDR;
  void *stack_addr = (void *)USER_PROG_STACK_ADDR;
  cpio_read_user_prog();
  asm volatile("mov x0, 1\n\t"
               "msr cntp_ctl_el0, x0\n\t"
               "mrs x0, cntfrq_el0\n\t"
               "msr cntp_tval_el0, x0\n\t"
               "mov x0, 2\n\t"
               "ldr x1, =0x40000040\n\t"
               "str w0, [x1]\n\t");

  asm volatile("mov x0, 0x0\n\t"
               "msr spsr_el1, x0\n\t");
  asm volatile("msr elr_el1, %0" : : "r"(addr));
  asm volatile("msr sp_el0, %0" : : "r"(stack_addr));
  asm volatile("eret");
  uart_puts("QQ");
}
