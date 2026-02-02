#pragma once

#define RV32IZicsr_CUSTOM_BUS

#include "rv32izicsr.h"

#include <stdint.h>

uint32_t External_RV32IZicsr_LoadHandlerU8(uint8_t *image, uint32_t offset);
uint32_t External_RV32IZicsr_LoadHandlerS8(uint8_t *image, uint32_t offset);
uint32_t External_RV32IZicsr_LoadHandlerU16(uint8_t *image, uint32_t offset);
uint32_t External_RV32IZicsr_LoadHandlerS16(uint8_t *image, uint32_t offset);
uint32_t External_RV32IZicsr_LoadHandlerU32(uint8_t *image, uint32_t offset);
void External_RV32IZicsr_StoreHandlerU8(uint8_t *image, uint32_t offset, uint32_t value);
void External_RV32IZicsr_StoreHandlerU16(uint8_t *image, uint32_t offset, uint32_t value);
void External_RV32IZicsr_StoreHandlerU32(uint8_t *image, uint32_t offset, uint32_t value);