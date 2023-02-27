#include "addr.h"
#include "uart.h"

void shell_image() {
  uart_puts("Please start sending...\r\n");

  unsigned int img_size = 0;
  for (int i = 0; i < 4; i++) {
    char c = uart_getc_raw();
    img_size <<= 8;
    img_size |= c;
  }

  uart_puts("Image size: ");
  uart_hex(img_size);
  uart_puts("\r\n");
  char *addr = TMP_KERNEL_ADDR;
  for (int i = 0; i < img_size; i++) {
    char c = uart_getc_raw();
    *(addr + i) = c;
  }

  uart_puts("Done");
  ((void (*)(void))addr)();
}
