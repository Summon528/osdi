#include "mbox.h"
#include "reset.h"
#include "uart.h"
#include "utils.h"

int main() {
  char *help = "help\tprint all available commands\r\n"
               "hello\tprint Hello World!\r\n"
               "mbox\tprint mbox info\r\n"
               "reset\treset\r\n";
  uart_init();
  char str[11] = {};
  unsigned short idx = 0;

  uart_puts("\r\nWelcome\r\n");
  while (1) {
    uart_puts("> ");

    while (1) {
      char c = uart_getc();
      if (c == 127) { // backspace
        idx = (idx <= 0 ? 0 : idx - 1);
        str[idx] = '\0';
        continue;
      }
      str[idx] = c;
      idx++;
      if (c == '\n' || idx >= 10) {
        uart_puts("\r\n");
        if (strncmp(str, "help", 4) == 0) {
          uart_puts(help);
        } else if (strncmp(str, "hello", 5) == 0) {
          uart_puts("Hello World!\n");
        } else if (strncmp(str, "mbox", 4) == 0) {
          mbox[0] = 7 * 4;
          mbox[1] = MBOX_REQUEST; // this is a request message
          mbox[2] = MBOX_TAG_GETBOARDREVISION;
          mbox[3] = 4; // buffer size
          mbox[4] = 0;
          mbox[5] = 0; // clear output buffer
          mbox[6] = MBOX_TAG_LAST;
          mbox_call(MBOX_CH_PROP);
          uart_puts("Board revision: ");
          uart_hex(mbox[5]);
          uart_puts("\r\n");

          mbox[0] = 8 * 4;
          mbox[1] = MBOX_REQUEST; // this is a request message
          mbox[2] = MBOX_TAG_GETARMMEMORY;
          mbox[3] = 8; // buffer size
          mbox[4] = 0;
          mbox[5] = 0; // clear output buffer
          mbox[6] = 0; // clear output buffer
          mbox[7] = MBOX_TAG_LAST;
          mbox_call(MBOX_CH_PROP);
          uart_puts("Base Address: ");
          uart_hex(mbox[5]);
          uart_puts("\r\nSize: ");
          uart_hex(mbox[6]);
          uart_puts("\r\n");
        } else if (strncmp(str, "reset", 5) == 0) {
          reset(1);
        } else {
          uart_puts("Unknown Command\n");
        }
        idx = 0;
        memset(str, 0, sizeof(str));
        break;
      } else {
        uart_send(c);
      }
    }
  }
}
