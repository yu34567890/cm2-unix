
# this is set by the root makefile
STAGING ?=
TOOLCHAIN ?=
KERNEL_HEADERS ?= 
LIBC ?=

CC = $(TOOLCHAIN)-gcc

CFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -fpie -fpic -nostdlib -nostartfiles -I$(KERNEL_HEADERS) -I$(LIBC)
LDFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -nostdlib -nostartfiles -T$(LIBC)/linker.ld $(LIBC)/*.o


all: clean ls install

ls.o: ls.c
	$(CC) $(CFLAGS) -c ls.c -o ls.o

ls: ls.o
	$(CC) $(LDFLAGS) ls.o -o ls.elf
	$(TOOLCHAIN)-objcopy -O binary ls.elf ls.bin

install:
	cp ls.bin $(STAGING)/bin/ls.bin

clean:
	rm -rf ls.o ls.elf



