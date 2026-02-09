#pragma once
#include <stdint.h>

#define TILEGPU_DRAW        1
#define TILEGPU_CLEAR       2

#define TILEGPU_NO_FX       0
#define TILEGPU_AND_COLOR   1
#define TILEGPU_NAND_COLOR  2
#define TILEGPU_XOR_COLOR   3
#define TILEGPU_OR_COLOR    4
#define TILEGPU_NOR_COLOR   5


#define TILEGPU_IOCTL_CLEAR 0
#define TILEGPU_IOCTL_DRAWTILE 1

struct tilegpu_ioctl_msg_drawtile {
    uint16_t tile_id;
    uint8_t x;
    uint8_t y;
    uint8_t controls;
};


