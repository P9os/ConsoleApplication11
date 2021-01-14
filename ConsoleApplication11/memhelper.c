#include "memhelper.h"

void* memcpyrev(void* buff1, void* buff2, size_t size)
{
    uint8_t* buf_dst = (uint8_t*)buff1 + size - 1;
    uint8_t* buf_src = (uint8_t*)buff2 + size - 1;
    while (size--)
    {
        *buf_dst-- = *buf_src--;
    }
    return buf_dst;
}
