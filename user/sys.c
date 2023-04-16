#include "sys.h"

void syscall(unsigned long x) {
  asm volatile("mov x0, %0\n\t"
               "svc 0\n\t"
               :
               : "r"(x)
               : "x0");
}

void syscall2(unsigned long x, unsigned long y) {
  asm volatile("mov x0, %0\n\t"
               "mov x1, %1\n\t"
               "svc 0\n\t"
               :
               : "r"(x), "r"(y)
               : "x0", "x1");
}

unsigned long getx0() {
  unsigned long x;
  asm volatile("mov %0, x0\n\t" : "=r"(x));
  return x;
}

int getpid() {
  syscall(SYS_GETPID);
  return getx0();
}

void exit() { syscall(SYS_EXIT); }

void write(const char *s) { syscall2(SYS_UART_WRITE, (unsigned long)s); }
