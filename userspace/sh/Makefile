
# this is set by the root makefile
STAGING ?=
TOOLCHAIN ?=
KERNEL_HEADERS ?= 
LIBC ?=

CC = $(TOOLCHAIN)-gcc

CFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -fpie -fpic -nostdlib -nostartfiles -I$(KERNEL_HEADERS) -I$(LIBC)
LDFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -nostdlib -nostartfiles -T$(LIBC)/linker.ld $(LIBC)/*.o


all: clean sh install

sh.o: sh.c
	$(CC) $(CFLAGS) -c sh.c -o sh.o

sh: sh.o
	$(CC) $(LDFLAGS) sh.o -o sh.elf
	$(TOOLCHAIN)-objcopy -O binary sh.elf sh.bin

install:
	cp sh.bin $(STAGING)/bin/sh.bin

clean:
	rm -rf sh.o sh.elf



