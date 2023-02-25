#include "help.h"
#include "uart.h"

static char *help = "help\tprint all available commands\r\n"
                    "hello\tprint Hello World!\r\n"
                    "mbox\tprint mbox info\r\n"
                    "reset\treset\r\n";

void shell_help() { uart_puts(help); }

void shell_hello() { uart_puts("Hello World!\n"); }
