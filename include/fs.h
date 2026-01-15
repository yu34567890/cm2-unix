#pragma once

#include "device.h"
#include <stdint.h>


struct superblock_t;
struct fd_t;
struct dentry_t;


#define FS_INAME_LEN 8
#define FS_PATH_LEN (FS_INAME_LEN*10)
struct inode_t {
    struct superblock_t* fs; //this is the filesystem that the inode is part of
    char name[FS_INAME_LEN]; //the name of the file
    uint8_t refcount; //the amount of references exist to this inode, this includes file descriptors and dentries
    uint8_t mode; //currently only stores filetype, but i will later expand it will actual permissions and ownership
};

struct stat {
    uint8_t mode;
    dev_t dev;
    uint32_t size;
    uint32_t time;
};

//these operate on file systems
struct super_ops_t {
    struct inode_t* (*lookup)(struct superblock_t* fs, struct inode_t* dir, char* name); //lookup an inode in a dir
    int (*evict)(struct superblock_t* fs, struct inode_t* i); //this is called by the dentry cache when it evicts a dentry, then the directory inode of that dir must also be "closed" this also means that if multiple dentries point to one inode it will still keep everything in check
    struct superblock_t* (*mount)(struct device_t* dev, char* args);
    int (*umount)(struct superblock_t* fs);
    int (*mkdir)(struct superblock_t* fs, struct inode_t* dir, char* name);
    int (*rmdir)(struct superblock_t* fs, struct inode_t* dir, char* name);
};

//these operate on file descriptors
struct file_ops_t {
    uint32_t (*read)(struct superblock_t* fs, struct fd_t* f, void* buffer, uint32_t count);
    uint32_t (*write)(struct superblock_t* fs, struct fd_t* f, void* buffer, uint32_t count);
    int (*lstat)(struct superblock_t* fs, struct fd_t* f, struct stat* statbuff);
    uint32_t (*lseek)(struct superblock_t* fs, struct fd_t* f, uint32_t offset, int whence);
    uint32_t (*close)(struct superblock_t* fs, struct fd_t* f);
    uint32_t (*readdir)(struct superblock_t* fs, struct fd_t* f, void* buffer, uint32_t count);
    uint32_t (*pread)(struct superblock_t* fs, struct fd_t* f, void* buffer, uint32_t count, uint32_t offset);
    uint32_t (*pwrite)(struct super_ops_t* fs, struct fd_t* f, void* biffer, uint32_t count, uint32_t offset);
};

struct superblock_t {
    struct super_ops_t* sops;
    struct file_ops_t* fops;
};

