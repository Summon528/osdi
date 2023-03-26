#include "eentry.h"
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

  uart_puts("\n");
}
