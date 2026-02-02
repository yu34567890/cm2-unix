#include <fs/vfs.h>

#include <stddef.h>
#include <fs/fs.h>
#include <kernel/proc.h>
#include <lib/stdlib.h>

//this is just a private type
#define FS_NAME_LEN 4
struct fs_tuple {
    struct super_ops* fs;
    char name[FS_NAME_LEN];
};

//this maps names to fileystem types
#define MAX_FILESYSTEM_COUNT 4
struct fs_tuple filesystem_registry[MAX_FILESYSTEM_COUNT];

#define MAX_FD 32
struct fd fd_table[MAX_FD];

void register_filesystem(const char* name, struct super_ops* fs)
{
    for (int i = 0; i < MAX_FILESYSTEM_COUNT; i++) {
        if (filesystem_registry[i].fs == NULL) {
            filesystem_registry[i].fs = fs;
            strncpy(filesystem_registry[i].name, (char*) name, FS_NAME_LEN);
            break;
        }
    }
}


//maybe there is a faster way of allocating these?
int fd_alloc()
{
    for (int i = 0; i < MAX_FD; i++) {
        if (fd_table[i].file == NULL) {
            return i;
        }
    }
    return -1;
}

void walk_path_init(path_walk_t* state, const char* path)
{
    strncpy(state->path_cpy, (char*) path, FS_PATH_LEN);
    state->current_word = state->path_cpy;
    state->path_ptr = state->path_cpy;
    state->dir = NULL;
}

uint8_t walk_path(path_walk_t* state)
{
    if (*state->path_ptr == '\0') {
        state->dir = state->fs->sops->lookup(state->fs, state->dir, state->current_word);
        return 1;
    }

    if (*state->path_ptr == '/') {
        *state->path_ptr = '\0';
        struct inode* next = state->fs->sops->lookup(state->fs, state->dir, state->current_word);
        state->dir = next;
        if (next == NULL) {
            return 2; //directory not found
        }
        
        state->current_word = state->path_ptr + 1;
    }
    state->path_ptr++;
    return 0;
}


void vfs_open(const char* path, uint8_t flags)
{
    struct proc* process = current_process;
    walk_path_init(&process->open_state.walker, path);
    process->open_state.walker.fs = NULL;
}

void vfs_open_update(struct proc* process)
{
    uint8_t rt = walk_path(&process->open_state.walker);
    if (rt == 2) { //directory not found
        
    } else if (rt == 1) { //we walked the entire path
        
    }
}


int vfs_read(int fd, void* buffer, uint32_t count)
{

}

int vfs_write(int fd, const void* buffer, uint32_t count)
{

}

int vfs_lstat(int fd, struct stat* statbuff)
{

}

int vfs_lseek(int fd, uint32_t offset, int whence)
{

}

void vfs_close(int fd)
{

}

int vfs_readdir(int fd, void* buffer, uint32_t count)
{

}

int vfs_pread(int fd, void* buffer, uint32_t count, uint32_t offset)
{

}

int vfs_pwrite(int fd, const void* buffer, uint32_t count, uint32_t offset)
{

}

void vfs_mount(const char* mount, const char* dev_path, const char* name)
{

}

void vfs_umount(const char* mount)
{

}

//we can also choose to only implement the *at versions of these syscalls since they are simpeler and more flexible, e.g mkdirat, rmdirat, unlinkat, these take a dirfd and a name
int vfs_mkdir(const char* path)
{

}

int vfs_rmdir(const char* path)
{

}

int vfs_unlink(const char* path)
{

}






