#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "global.h"

void *smalloc(size_t size) {
    void *alloc = malloc(size);
    if (!alloc)
        app_abort("smalloc()", "Failed to allocate memory")
    return alloc;
}

void *srealloc(void *ptr, size_t size) {
    void *alloc = realloc(ptr, size);
    if (!alloc)
        app_abort("srealloc()", "Failed to allocate memory")
    return alloc;    
}

void *scalloc(size_t nmemb, size_t size) {
    void *alloc = calloc(nmemb, size);
    if (!alloc)
        app_abort("scalloc()", "Failed to allocate memory")
    return alloc;
}

int random_int(int min, int max) {
    srand(time(NULL));
    return min + rand() % (max + 1  - min);
}
