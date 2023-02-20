TOOLCHAIN_PREFIX = aarch64-suse-linux-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles

all: kernel8.img

start.o: start.S
	$(CC) $(CFLAGS) -c start.S -o start.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel8.img: start.o $(OBJS)
	$(LD) start.o $(OBJS) -T link.ld -o kernel8.elf
	$(OBJCPY) -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.img kernel8.elf *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio
