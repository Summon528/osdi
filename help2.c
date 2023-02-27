#include "help2.h"
#include "addr.h"
#include "uart.h"

int mem_used = 0;

void *simple_malloc(unsigned int size) {
  uart_hex((unsigned int)HEAP_ADDR);
  uart_puts("\n");
  uart_hex(mem_used);
  void *r = HEAP_ADDR + mem_used;
  mem_used += size;

  uart_puts("OK\n");
  return r;
}

void shell_hello2() {
  char *string = simple_malloc(7);
  uart_hex((unsigned int)string);
  uart_puts("\n");
  string[0] = 'h';
  string[1] = 'e';
  string[2] = 'l';
  string[3] = 'l';
  string[4] = 'o';
  string[5] = '\n';
  string[6] = '\0';
  uart_puts(string);
  uart_puts("Memory usage: ");
  uart_hex(mem_used);
  uart_puts("\n");
}
