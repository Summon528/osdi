#define SYS_GETPID 10
#define SYS_EXIT 11
#define SYS_UART_WRITE 12
#define SYS_FORK 13

#ifndef __ASSEMBLY__
int getpid();
void write(const char *);
void exit();
int fork();
#endif
