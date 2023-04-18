TOOLCHAIN_PREFIX = aarch64-suse-linux-
CC = $(TOOLCHAIN_PREFIX)gcc
LD = $(TOOLCHAIN_PREFIX)ld
OBJCPY = $(TOOLCHAIN_PREFIX)objcopy

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -ffreestanding -nostdinc -nostdlib -nostartfiles -g

all: user.img

start.o: start.S
	$(CC) $(CFLAGS) -c start.S -o start.o

sys.o: sys.S
	$(CC) $(CFLAGS) -c sys.S -o sys.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

user.img: start.o sys.o $(OBJS)
	$(LD) start.o sys.o $(OBJS) -T link.ld -o user.elf
	$(OBJCPY) -O binary user.elf user.img

clean:
	rm user.img user.elf *.o >/dev/null 2>/dev/null || true

