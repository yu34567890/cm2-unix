#pragma once

#include <stdint.h>
#include <stdbool.h>

void Tty_Init(void);
void Tty_Tick(void);

/* below are for custom_bus.c */

bool Tty_OffsetInteraction(uint32_t offset);
uint32_t Tty_ByteLoad(uint32_t offset);
void Tty_ByteStore(uint32_t offset, uint32_t value);