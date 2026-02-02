#include "rv32izicsr.h"

#include "mmio/tty.h"
#include "mmio/rng.h"
#include "mmio/disk.h"
#include "mmio/tilegpu.h"

uint32_t External_RV32IZicsr_LoadHandlerU8(uint8_t *image, uint32_t offset) {
    if (Tty_OffsetInteraction(offset)) {
        return Tty_ByteLoad(offset);
    }
    if (Rng_OffsetInteraction(offset)) {
        return Rng_ByteLoad(offset);
    }
    if (Disk_OffsetInteraction(offset)) {
        return Disk_ByteLoad(offset);
    }
    return ((uint32_t)*(uint8_t *)(image + (offset & RV32IZicsr_RAM_MASK)));
}

uint32_t External_RV32IZicsr_LoadHandlerS8(uint8_t *image, uint32_t offset) {
    if (Tty_OffsetInteraction(offset)) {
        return Tty_ByteLoad(offset);
    }
    if (Rng_OffsetInteraction(offset)) {
        return Rng_ByteLoad(offset);
    }
    if (Disk_OffsetInteraction(offset)) {
        return Disk_ByteLoad(offset);
    }
    return ((int32_t)*(int8_t *)(image + (offset & RV32IZicsr_RAM_MASK)));
}

uint32_t External_RV32IZicsr_LoadHandlerU16(uint8_t *image, uint32_t offset) {
    return ((uint32_t)*(uint16_t *)(image + (offset & RV32IZicsr_RAM_MASK)));
}

uint32_t External_RV32IZicsr_LoadHandlerS16(uint8_t *image, uint32_t offset) {
    return ((int32_t)*(int16_t *)(image + (offset & RV32IZicsr_RAM_MASK)));
}

uint32_t External_RV32IZicsr_LoadHandlerU32(uint8_t *image, uint32_t offset) {
    return (*(uint32_t *)(image + (offset & RV32IZicsr_RAM_MASK)));
}

void External_RV32IZicsr_StoreHandlerU8(uint8_t *image, uint32_t offset, uint32_t value) {
    if (Tty_OffsetInteraction(offset)) {
        Tty_ByteStore(offset, value);
    }
    if (Disk_OffsetInteraction(offset)) {
        return Disk_ByteStore(offset, value);
    }
    if (TileGpu_OffsetInteraction(offset)) {
        return TileGpu_ByteStore(offset, value);
    }
    (*(uint8_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value);
}

void External_RV32IZicsr_StoreHandlerU16(uint8_t *image, uint32_t offset, uint32_t value) {
    if (Disk_OffsetInteraction(offset)) {
        return Disk_HalfStore(offset, value);
    }
    if (TileGpu_OffsetInteraction(offset)) {
        return TileGpu_HalfStore(offset, value);
    }
    (*(uint16_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value);
}

void External_RV32IZicsr_StoreHandlerU32(uint8_t *image, uint32_t offset, uint32_t value) {
    (*(uint32_t *)(image + (offset & RV32IZicsr_RAM_MASK)) = value);
}