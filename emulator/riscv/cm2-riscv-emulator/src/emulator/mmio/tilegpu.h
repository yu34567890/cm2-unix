#pragma once

#include <stdint.h>

void TileGpu_Init(const char *bitmap_filename);
bool TileGpu_OffsetInteraction(uint32_t offset);
void TileGpu_ByteStore(uint32_t offset, uint32_t value);
void TileGpu_HalfStore(uint32_t offset, uint32_t value);
