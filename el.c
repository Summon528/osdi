#include "uart.h"

void shell_el() {
  unsigned long el;
  asm volatile("mrs %0, CurrentEL" : "=r"(el));
  uart_puts("CurrentEL: ");
  uart_hex((el >> 2) & 0b11);
  uart_puts("\n");
}
