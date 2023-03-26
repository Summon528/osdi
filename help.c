#include "help.h"
#include "uart.h"

static char *help = "help\tprint all available commands\r\n"
                    "hello\tprint Hello World!\r\n"
                    "mbox\tprint mbox info\r\n"
                    "reset\treset\r\n"
                    "image\tflash and boot an image\r\n"
                    "malloc\tsimple malloc\r\n"
                    "ls\tls ramdisk\r\n"
                    "cat\tcat ramdisk\r\n"
                    "el\tshow current el\r\n"
                    "exec\texecute user.img\r\n";

void shell_help() { uart_puts(help); }

void shell_hello() { uart_puts("Hello World!\n"); }
