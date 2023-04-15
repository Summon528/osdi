
void core_timer_enable() {
  asm volatile("mov x0, 1\n\t"
               "msr cntp_ctl_el0, x0\n\t"
               "mrs x0, cntfrq_el0\n\t"
               "msr cntp_tval_el0, x0\n\t"
               "mov x0, 2\n\t"
               "ldr x1, =0x40000040\n\t"
               "str w0, [x1]\n\t");
}
