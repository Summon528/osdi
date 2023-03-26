#include "addr.h"
#include "uart.h"
#include "utils.h"

struct cpio_newc_header {
  char c_magic[6];
  char c_ino[8];
  char c_mode[8];
  char c_uid[8];
  char c_gid[8];
  char c_nlink[8];
  char c_mtime[8];
  char c_filesize[8];
  char c_devmajor[8];
  char c_devminor[8];
  char c_rdevmajor[8];
  char c_rdevminor[8];
  char c_namesize[8];
  char c_check[8];
};

unsigned int hex_to_uint(char hex[8]) {
  unsigned int r = 0;
  unsigned int base = 1;
  for (int i = 7; i >= 0; i--) {
    int digit = hex[i] <= '9' ? hex[i] - '0' : hex[i] - 'A' + 10;
    r += digit * base;
    base *= 16;
  }
  return r;
}

unsigned int round_to_4(unsigned int x) { return ((x - 1) / 4 + 1) * 4; }

void lscat(short do_cat) {
  void *addr = (void *)INITRAMFS_ADDR;
  while (1) {
    struct cpio_newc_header *hdr = (struct cpio_newc_header *)addr;
    unsigned int fsize = hex_to_uint(hdr->c_filesize);
    unsigned int nsize = hex_to_uint(hdr->c_namesize);
    void *content_addr =
        addr + round_to_4(sizeof(struct cpio_newc_header) + nsize);
    if (nsize == 0xB && fsize == 0 &&
        strncmp(content_addr, "TRAILER!!!", 0xB)) {
      break;
    }
    addr += sizeof(struct cpio_newc_header);
    if (do_cat) {
      uart_puts("Filename: ");
    }
    for (int i = 0; i < nsize; i++) {
      uart_send(*(char *)(addr + i));
    }
    uart_puts("\n");
    if (do_cat) {
      for (int i = 0; i < fsize; i++) {
        uart_send(*(char *)(content_addr + i));
      }
      uart_puts("\n");
    }
    addr = content_addr + round_to_4(fsize);
  }
}

void cpio_read_user_prog() {
  void *addr = (void *)INITRAMFS_ADDR;
  while (1) {
    struct cpio_newc_header *hdr = (struct cpio_newc_header *)addr;
    unsigned int fsize = hex_to_uint(hdr->c_filesize);
    unsigned int nsize = hex_to_uint(hdr->c_namesize);
    void *content_addr =
        addr + round_to_4(sizeof(struct cpio_newc_header) + nsize);
    if (nsize == 0xB && fsize == 0 &&
        strncmp(content_addr, "TRAILER!!!", 0xB)) {
      break;
    }
    addr += sizeof(struct cpio_newc_header);
    if (strncmp((char *)addr, "user.img", nsize) == 0) {
      uart_puts("Found!");
      for (int i = 0; i < fsize; i++) {
        *((char *)USER_PROG_ADDR + i) = *((char *)content_addr + i);
      }
    }
    addr = content_addr + round_to_4(fsize);
  }
}
void shell_cpio_ls() { lscat(0); }
void shell_cpio_cat() { lscat(1); }
