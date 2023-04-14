TOOLCHAIN_PREFIX = aarch64-suse-linux-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy

SRCS = $(wildcard *.c)
ASM_SRCS = $(wildcard *.S)
OBJS = $(SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.S=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -g

all: kernel8.img bootloader cpio user

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.img: $(ASM_OBJS) $(OBJS)
	$(LD) $(ASM_OBJS) $(OBJS) -T link.ld -o kernel8.elf
	$(OBJCPY) -O binary kernel8.elf kernel8.img

bootloader.img: $(ASM_OBJS) $(OBJS)
	$(LD) $(ASM_OBJS) $(OBJS) -T link.bootloader.ld -o bootloader.elf
	$(OBJCPY) -O binary bootloader.elf bootloader.img

kernel: kernel8.img

bootloader: bootloader.img

user: FORCE
	make -C ./user

cpio: user
	cp ./user/user.img ./rootfs/
	cd rootfs && find . | cpio -o -H newc > ../initramfs.cpio && cd ..

clean:
	rm *.cpio kernel8.img kernel8.elf rootfs/user.img *.o >/dev/null 2>/dev/null || true
	make -C ./user clean

run: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio -s -initrd initramfs.cpio

run-pty: bootloader
	qemu-system-aarch64 -M raspi3b -kernel bootloader.img -serial null -serial pty -s -initrd initramfs.cpio

FORCE:
