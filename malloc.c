#include "malloc.h"
#include "addr.h"
#include "uart.h"

int mem_used = 0;

void *simple_malloc(unsigned int size) {
  void *r = HEAP_ADDR + mem_used;
  mem_used += size;
  return r;
}

void shell_malloc() {
  char *string = simple_malloc(7);
  string[0] = 'h';
  string[1] = 'e';
  string[2] = 'l';
  string[3] = 'l';
  string[4] = 'o';
  string[5] = '\n';
  string[6] = '\0';
  uart_puts("Memory usage: ");
  uart_hex(mem_used);
  uart_puts("\n");
}
