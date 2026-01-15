#include <stddef.h>
#include "dcache.h"
#include "../lib/stdlib.h"


struct dentry_t dentry_pool[FS_DENTRY_POOL_LEN];

uint8_t dentry_free_list[FS_DENTRY_POOL_LEN];
uint8_t dentry_free_list_top;


void dentry_evict(uint8_t dentry)
{
    struct dentry_t* den = &dentry_pool[dentry];
    struct inode_t* dir = den->i;
    if (den->refcount <= 2) {//refcount of 2 means that it only has a connection with the previous dentry and a connection with the inode representing the directory
        dir->fs->sops->evict(dir->fs, dir);
        den->i = NULL;
        den->refcount--;
        struct dentry_t* prev = &dentry_pool[den->prev];
        
        //remove old reference
        for (int k = 1; k < FS_DENTRY_POOL_LEN; k++) {
            if (prev->next[k] == dentry) {
                prev->next[k] = 0;
            }
        }
        prev->refcount--;
        den->prev = 0;
        den->refcount = 0;

    }
}

int dentry_alloc()
{
    uint8_t tmp = dentry_free_list_top;
    if (tmp == 0) { //no more free dentries left
        
    } else {
        uint8_t free_dentry = dentry_free_list[--tmp];
        dentry_free_list_top = tmp;
        return free_dentry;
    }
}

struct dentry_t* dentry_lookup(const char* path)
{ 
    char path_cpy[FS_PATH_LEN] = {0};
    strncpy(path_cpy, (char*) path, FS_PATH_LEN);
    char* path_ptr = path_cpy;
    char* current_word = path_cpy;
    
    uint8_t current_dentry = 0;

    //parse the path and lookup each dir from within the previous dir
    while(*path_ptr != '\0') {
        if (*path_ptr == '/') {
            *path_ptr = '\0';
            current_dentry = dentry_local_lookup(current_dentry, current_word);
            if (current_dentry == 0) { //0 is reserved for the root, so it can never be returned
                return -1;
            }
            current_word = path_ptr + 1;
        }
        path_ptr++;
    }

    current_dentry = dentry_local_lookup(current_dentry, current_word);
    if (current_dentry == 0) {
        return NULL;
    }
    return &dentry_pool[current_dentry];
}

uint8_t dentry_local_lookup(uint8_t dentry_index, char* name)
{
    struct dentry_t* dentry = &dentry_pool[dentry_index];
    for (int i = 0; i < FS_DENTRY_MAXCHILDREN; i++) {
        uint8_t child = dentry->next[i];
        if (child != 0) {
            struct dentry_t* next = &dentry_pool[child];
            if (strncmp(next->i->name, name, FS_INAME_LEN) == 0) {
                return child;
            }
        }
    }
    
    //if the name is not found we create a new dentry
    struct inode_t* dir = NULL; //the dir is NULL if it is the root of a fs
    if (dentry->i->fs == dentry->mount) { //if the fs of the dir and the fs of the dentry are not the same its a mountpoint
        dir = dentry->i;
    }
    struct inode_t* i = dentry->mount->sops->lookup(dentry->mount, dir, name);
    if (i == NULL) {
        return 0;
    }

    int new_dentry = dentry_alloc();
    if (new_dentry < 0) {
        return 0;
    }
    struct dentry_t* new = &dentry_pool[new_dentry];
    new->i = i;
    new->mount = dentry->mount;
    new->prev = dentry_index;
    memset(new->next, FS_DENTRY_MAXCHILDREN, 0);
    return new_dentry;
}

int dentry_mount(struct superblock_t* fs, const char* mountpoint) {

    struct dentry_t* mount = dentry_lookup(mountpoint);
    mount->mount = fs;
    mount->refcount++; //increase the refcount by one to prevent the dentry from being evicted
}



