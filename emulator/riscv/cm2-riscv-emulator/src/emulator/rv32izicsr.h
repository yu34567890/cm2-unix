#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Configuration */
#define RV32IZicsr_RAM_SIZE 65536
#define RV32IZicsr_RAM_MASK (RV32IZicsr_RAM_SIZE - 1)
#include "custom_bus.h"

#ifndef RV32IZicsr_CUSTOM_BUS
   #define RV32IZicsr_LoadU8(image, offset) ((uint32_t)*(uint8_t *)(image + (offset & RV32IZicsr_RAM_MASK)))
   #define RV32IZicsr_LoadS8(image, offset) ((int32_t)*(int8_t *)(image + (offset & RV32IZicsr_RAM_MASK)))
   #define RV32IZicsr_LoadU16(image, offset) ((uint32_t)*(uint16_t *)(image + (offset & RV32IZicsr_RAM_MASK)))
   #define RV32IZicsr_LoadS16(image, offset) ((int32_t)*(int16_t *)(image + (offset & RV32IZicsr_RAM_MASK)))
   #define RV32IZicsr_LoadU32(image, offset) (*(uint32_t *)(image + (offset & RV32IZicsr_RAM_MASK)))

   #define RV32IZicsr_StoreU8(image, offset, value) (*(uint8_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value)
   #define RV32IZicsr_StoreU16(image, offset, value) (*(uint16_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value)
   #define RV32IZicsr_StoreU32(image, offset, value) (*(uint32_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value)
#else
   #define RV32IZicsr_LoadU8(image, offset) External_RV32IZicsr_LoadHandlerU8(image, offset);
   #define RV32IZicsr_LoadS8(image, offset) External_RV32IZicsr_LoadHandlerS8(image, offset);
   #define RV32IZicsr_LoadU16(image, offset) External_RV32IZicsr_LoadHandlerU16(image, offset);
   #define RV32IZicsr_LoadS16(image, offset) External_RV32IZicsr_LoadHandlerU16(image, offset)
   #define RV32IZicsr_LoadU32(image, offset) External_RV32IZicsr_LoadHandlerU32(image, offset);

   #define RV32IZicsr_StoreU8(image, offset, value) External_RV32IZicsr_StoreHandlerU8(image, offset, value)
   #define RV32IZicsr_StoreU16(image, offset, value) External_RV32IZicsr_StoreHandlerU16(image, offset, value)
   #define RV32IZicsr_StoreU32(image, offset, value) External_RV32IZicsr_StoreHandlerU32(image, offset, value)
#endif

struct RV32IZicsr_State {
   uint32_t regs[32];
   uint32_t pc;

   uint32_t csrs[4096];
};

void RV32IZicsr_InitState(struct RV32IZicsr_State *state);
void RV32IZicsr_Step(struct RV32IZicsr_State *state, uint8_t *image);
bool RV32IZicsr_ImageRead(void);
uint32_t RV32IZicsr_GetImageReadAddr();
