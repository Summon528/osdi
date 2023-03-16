#include "cpio.h"
#include "el.h"
#include "help.h"
#include "image.h"
#include "malloc.h"
#include "mbox.h"
#include "reset.h"
#include "uart.h"
#include "utils.h"

typedef struct cmd {
  char *word;
  void (*handler)();
} cmd_t;

static cmd_t cmds[] = {
    {"help", shell_help},    {"hello", shell_hello},   {"mbox", shell_mbox},
    {"reset", shell_reset},  {"image", shell_image},   {"ls", shell_cpio_ls},
    {"cat", shell_cpio_cat}, {"malloc", shell_malloc}, {"el", shell_el}};

int main() {
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

        short handled = 0;
        for (int i = 0; i * sizeof(cmd_t) < sizeof(cmds); i++) {
          if (strncmp(str, cmds[i].word, strlen(cmds[i].word)) == 0) {
            (*cmds[i].handler)();
            handled = 1;
            break;
          }
        }
        if (!handled) {
          uart_puts("Unknown Command\n");
        }

        idx = 0;
        memset(str, 0, sizeof(str));
        break;
      }

      uart_send(c);
    }
  }
}
