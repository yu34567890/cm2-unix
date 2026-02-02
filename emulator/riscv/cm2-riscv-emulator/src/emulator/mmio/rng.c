#include <stdint.h>
#include <stdbool.h>

#include "../../util.h"
#include "mmio_map.h"

bool Rng_OffsetInteraction(uint32_t offset) {
    if (offset == RANDOM_BYTE) return true;
    return false;
}

uint32_t Rng_ByteLoad(uint32_t offset) {
    switch (offset) {
        case RANDOM_BYTE:
            return random_int(0, 255);
    }
    return 0;
}