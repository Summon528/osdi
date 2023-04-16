#include "addr.h"
#include "cpio.h"
#include "task.h"
#include "uart.h"

void exec_user() {
  void *addr = (void *)USER_PROG_ADDR;
  void *stack_addr = (void *)USER_PROG_STACK_ADDR + task_getid() * 10000;
  cpio_read_user_prog();

  asm volatile("mov x0, 0x0\n\t"
               "msr spsr_el1, x0\n\t");
  asm volatile("msr elr_el1, %0" : : "r"(addr));
  asm volatile("msr sp_el0, %0" : : "r"(stack_addr));
  asm volatile("eret");
}

void shell_exec() { task_create(exec_user); }
