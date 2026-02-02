#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "mmio_map.h"

bool Disk_OffsetInteraction(uint32_t offset) {
    if (offset == DISK_ADDRESS) return true;
    if (offset == DISK_DATA) return true;
    if (offset == DISK_OUT) return true;
    if (offset == DISK_WRITE) return true;
    return false;
}

static uint8_t disk[65536] = {0};

static uint16_t disk_address;
static uint16_t disk_data;

void Disk_LoadBin(const char *filename) {
    FILE *fptr = fopen(filename, "rb");
    fread(disk, 1, sizeof(disk), fptr);
    fclose(fptr);
}

uint32_t Disk_ByteLoad(uint32_t offset) {
    switch (offset) {
        case DISK_OUT:
            return disk[disk_address];
    }
    return 0;
}

static void dump_disk_as_file(void) {
    FILE *fptr = fopen("disk_dump.bin", "wb");
    fseek(fptr, 0, SEEK_SET);
    fwrite(disk, 1, sizeof(disk), fptr);
    fclose(fptr);
}

void Disk_ByteStore(uint32_t offset, uint32_t value) {
    switch (offset) {
        case DISK_DATA:
            disk_data = value;
            break;
        case DISK_WRITE:
            disk[disk_address] = disk_data;
            dump_disk_as_file();
    }
}

void Disk_HalfStore(uint32_t offset, uint32_t value) {
    switch (offset) {
        case DISK_ADDRESS:
            disk_address = value;
    }
}