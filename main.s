# 0 "main.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/aarch64-suse-linux/sys-root/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "main.S"
.section ".text"
_start:
  wfe
  b _start
