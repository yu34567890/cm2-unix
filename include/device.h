#pragma once
#include "fs.h"

typedef uint8_t dev_t;

#define MAJOR(DEVNO) (DEVNO >> 4)
#define MINOR(DEVNO) (DEVNO & 0xF)

#define MKDEV(MAJOR_NO, MINOR_NO) ((MAJOR_NO << 4) | (MINOR_NO & 0xF))

struct device_t;

struct device_driver_t {
    struct device_t* (*create)(uint8_t* minor, const void* args); //returns minor number or a negative number on failure
    int (*destroy)(uint8_t minor);
    struct device_t* (*lookup)(uint8_t minor);
    const char* name;
};

struct device_ops_t {
    uint32_t (*read)(struct device_t* dev, void* buffer, uint32_t count, uint32_t offset);
    uint32_t (*write)(struct device_t* dev, void* buffer, uint32_t count, uint32_t offset);
    int (*ioctl)(struct device_t* dev, int cmd, void* arg);
};

struct device_t {
    struct device_ops_t* ops;
};

#define DEVICE_DRIVER_MAX 16
extern device_driver_t driver_registry[DEVICE_DRIVER_MAX];

struct device_t* device_create(uint8_t major, const void* args);
struct device_t* device_lookup(dev_t dev);
int device_destroy(dev_t dev);

