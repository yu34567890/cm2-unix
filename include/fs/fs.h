#pragma once

#include <kernel/device.h>
#include <stdint.h>

struct superblock;
struct fd;
struct dentry;

typedef uint32_t inum_t; //this is a unique identifier for an inode, this would be like the cluster + offset in fat

#define FS_INAME_LEN 10
#define FS_PATH_LEN (FS_INAME_LEN*4)
#define FS_MODE_DIR 1
#define FS_MODE_FILE 2
#define FS_MODE_MOUNT 3

#define INODE_TABLE_SIZE 32

struct romfs_inode {
    void* data;
    uint32_t length;
};


struct inode {
    struct superblock* fs; //this is the filesystem that the inode is part of
    char name[FS_INAME_LEN]; //the name of the file
    uint8_t refcount; //the amount of references exist to this inode, this includes file descriptors and dentries
    uint8_t mode; //currently only stores filetype, but i will later expand it will actual permissions and ownership
    inum_t dir; //the parent directory of this inode
    inum_t file; //the inum of this inode
    union {
        struct romfs_inode romfs;
    };
};

struct stat {
    uint32_t size;
    uint32_t time;
    uint8_t mode;
    dev_t dev;
};

typedef struct {
    struct device_request* req;
    struct superblock* fs;
    struct inode* dir;
    char* fname;
} fs_lookup_t;

typedef struct {
    struct device_driver* req;
    struct superblock* fs;
    struct fd* descriptor;
    uint32_t bytes_read;
    uint32_t count;
    void* buffer;
} fs_read_t;

//these operate on file systems
struct super_ops {
    int8_t (*lookup)(fs_lookup_t* state); //lookup an inode in a dir
    int (*release)(struct superblock* fs, struct inode* i); //this decrements the refcount and eventually free's the inode
    struct superblock* (*mount)(struct device* dev, const char* args);
    int (*umount)(struct superblock* fs);
    int (*mkdir)(struct superblock* fs, struct inode* dir, char* name);
    int (*rmdir)(struct superblock* fs, struct inode* dir, char* name);
    int (*unlink)(struct superblock* fs, struct inode* dir, char* name);
};

//these operate on file descriptors
struct file_ops {
    int8_t (*read)(fs_read_t* state);
    int8_t (*write)(fs_read_t* state);
    int (*lstat)(struct superblock* fs, struct fd* f, struct stat* statbuff);
    int (*lseek)(struct superblock* fs, struct fd* f, uint32_t offset, int whence);
    void (*close)(struct superblock* fs, struct fd* f);
    int (*readdir)(struct superblock* fs, struct fd* f, void* buffer, uint32_t count);
    int (*pread)(struct superblock* fs, struct fd* f, void* buffer, uint32_t count, uint32_t offset);
    int (*pwrite)(struct super_ops* fs, struct fd* f, void* biffer, uint32_t count, uint32_t offset);
};

struct superblock {
    struct super_ops* sops;
    struct file_ops* fops;
};

//file flag defenitions
#define FD_R (1 << 0)
#define FD_W (1 << 1)
#define FD_CREAT (1 << 2)
#define FD_NONBLOCK (1 << 3)

struct fd {
    struct inode* file;
    uint32_t offset : 24;
    uint8_t flags : 8;
};

#define MAX_FILESYSTEM_COUNT 4
#define FS_NAME_LEN 4
#define MAX_FD 32
extern struct fd fd_table[MAX_FD];


void fs_init();
struct inode* create_inode();
void free_inode(struct inode* i);
int8_t lookup_dir(fs_lookup_t* state);
void register_filesystem(const char* name, struct super_ops* fs);
struct super_ops* lookup_filesystem(const char* name);
int fd_alloc();



