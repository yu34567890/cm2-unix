#pragma once

#include <stddef.h>

void *smalloc(size_t size);
void *srealloc(void *ptr, size_t size);
void *scalloc(size_t nmemb, size_t size);
int random_int(int min, int max);