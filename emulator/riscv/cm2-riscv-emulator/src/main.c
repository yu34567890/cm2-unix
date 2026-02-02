#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "util.h"

#include "emulator/rv32izicsr.h"
#include "emulator/mmio/tty.h"
#include "emulator/mmio/tilegpu.h"
#include "emulator/mmio/disk.h"

static void load_bin_file(const char *filename, void *dest, size_t dest_size) {
    FILE *fptr = fopen(filename, "r");
    if (!fptr) {
        app_abort("load_bin_file()", "Unable to obtain file handle")
    }

    fseek(fptr, 0, SEEK_END);
    rewind(fptr);
    fread(dest, 1, dest_size - 1, fptr);
    fclose(fptr);
}

struct RV32IZicsr_State state;
uint8_t *image = NULL;

int main(int argc, char **argv) {
    if (argc <= 1) {
        quick_abort("Usage: cm2-riscv-emulator <Filepath to initial .bin> <Filepath to tilegpu .bmp> <Filepath to disk .bin image>")
    }

    image = scalloc(1, RV32IZicsr_RAM_SIZE);

    /* Init RV32I */
    load_bin_file(argv[1], image, RV32IZicsr_RAM_SIZE);
    RV32IZicsr_InitState(&state);

    /* Init MMIO Devices */
    Tty_Init();

    /* Init TileGPU and Disk */
    if (argc == 4) {
        TileGpu_Init(argv[2]);
        Disk_LoadBin(argv[3]);
    } else if (argc == 3)
        Disk_LoadBin(argv[2]);

    while (1) {
        Tty_Tick();
        RV32IZicsr_Step(&state, image);
    }
}