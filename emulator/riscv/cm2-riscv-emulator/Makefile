CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -lncurses -lraylib -fsanitize=address \
	-Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=parentheses

ROOT ?= .
BUILD = $(ROOT)/build
OUTPUT = $(BUILD)/cm2-riscv-emulator
OUTPUT_ARGS ?= emulator-bin/minesweeper.bin emulator-tilesheet/minesweeper.bmp

CSRCS = $(shell find $(ROOT)/src -name '*.c')
OBJS = $(patsubst %.c,$(BUILD)/%.o,$(CSRCS))

all: clean compile

force:

compile: $(OBJS) $(OUTPUT)

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(OUTPUT)

run:
	./$(OUTPUT) $(OUTPUT_ARGS)

clean:
	rm -rf $(BUILD)
