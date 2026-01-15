#pragma once
#include "../include/fs.h"

#define FS_DENTRY_MAXCHILDREN 10
struct dentry_t {
    uint8_t next[FS_DENTRY_MAXCHILDREN];
    uint8_t prev;
    uint8_t refcount;
    struct inode_t* i; //this holds the inode of this dir
    struct superblock_t* mount; //this holds a mountpoint
};

#define FS_DENTRY_POOL_LEN 10
extern struct dentry dentry_pool[FS_DENTRY_POOL_LEN];

extern uint8_t dentry_free_list[FS_DENTRY_POOL_LEN];
extern uint8_t dentry_free_list_top;


uint8_t dentry_local_lookup(uint8_t dentry_index, char* name);
struct dentry_t* dentry_lookup(const char* path);

