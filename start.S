/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, go
    b loop // cpu id > 0, stop

go:  // cpu id == 0

    bl from_el2_to_el1
    bl set_exception_vector_table

    // clear bss
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
    
clear_bss_start:
    cbz     w2, clear_bss_done
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clear_bss_start
    
clear_bss_done:
    // set top of stack just before our code (stack grows to a lower address per AAPCS64)
    ldr     x1, =_start
    mov     sp, x1

    bl      main // jump to C code, should not return
    b       loop    // for failsafe, halt this core too

from_el2_to_el1:
    mov x0, (1 << 31)       // EL1 uses aarch64
    msr hcr_el2, x0
    mov x0, 0x3c5           // EL1h with interrupt disabled
    msr spsr_el2, x0
    msr elr_el2, lr
    eret                    // return to EL1
    
set_exception_vector_table:
  adr x0, exception_vector_table
  msr vbar_el1, x0
  ret

loop:
    wfe
    b loop
