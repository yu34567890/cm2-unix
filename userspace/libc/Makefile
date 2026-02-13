
# this is set by the root makefile
STAGING ?=
TOOLCHAIN ?=
#kernel headers
KERNEL_HEADERS ?= 

CC = $(TOOLCHAIN)-gcc

CFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -fpie -fpic -nostdlib -nostartfiles -I$(KERNEL_HEADERS)
LDFLAGS = -march=rv32i -mabi=ilp32 -Os -flto -ffreestanding -static -nostdlib -nostartfiles -Tlinker.ld

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o) start.o


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all libc clean

all: clean libc

libc: $(OBJS)

clean:
	rm -rf $(OBJS)
