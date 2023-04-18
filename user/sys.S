#define __ASSEMBLY__
#include "sys.h"

.global getpid
getpid:
    mov x0, SYS_GETPID
    svc #0
    ret


.global write
write:
    mov x1, x0
    mov x0, SYS_UART_WRITE
    svc #0
    ret

.global exit
exit:
    mov x0, SYS_EXIT
    svc #0
    ret

