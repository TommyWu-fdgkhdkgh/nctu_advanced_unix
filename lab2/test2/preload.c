// preload.c
#include <stdio.h>
#include <stdlib.h>

void* malloc(size_t size)
{
    //我猜是printf有用到malloc吧，所以在printf的時候就掛掉了
    //printf("%s size: %lu\n", __func__, size);
    //puts("gg\n");
    __libc_malloc(size);
    return NULL;
}
