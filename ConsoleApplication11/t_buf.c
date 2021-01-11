#include "t_buf.h"

static uint8_t isInit = 0;
static uint8_t* iBuffer;
static int32_t BufferMapParent[32] = { 0 };

static void Init();
static int8_t GetAvailBlock();

bool Get_Buffer(t_buf* buffer)
{
    if (!isInit)
    {
        Init();
    }
    buffer->data = iBuffer;
    buffer->totalSize = BUFFER_SIZE;
    buffer->numberOfParent = rand();
    return 1;
}

bool Write_Buffer(t_buf* buffer, uint8_t* data, uint16_t size)
{
    if (size > BUFFER_SIZE) return 0;
    uint8_t nReqBlocks = size / BLOCK_SIZE;
    if (nReqBlocks == 0)
    {
        int8_t nBlockAvail = GetAvailBlock();
        if (nBlockAvail == -1) return 0;
        uint8_t* addr = &iBuffer[nBlockAvail * BLOCK_SIZE];
        memcpy(addr, data, size);
        BufferMapParent[nBlockAvail] = buffer->numberOfParent;
        buffer->data = addr;
        return 1;
    }
    else
    {
        nReqBlocks += 1;
        while (nReqBlocks)
        {
            uint8_t* d = data;
            int8_t nBlockAvail = GetAvailBlock();
            if (nBlockAvail == -1) return 0;
            uint8_t* addr = &iBuffer[nBlockAvail * BLOCK_SIZE];
            memcpy(addr, d, BLOCK_SIZE);
            BufferMapParent[nBlockAvail] = buffer->numberOfParent;
            buffer->data = addr;
            d += BLOCK_SIZE;
            nReqBlocks--;
        }
        return 1;
    }
}

bool Write_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
    if (size > BUFFER_SIZE) return 0;
    uint8_t nReqBlock = size / BLOCK_SIZE;
    nReqBlock += 1;
    while (nReqBlock)
    {
        int8_t nBlockAvail = GetAvailBlock();
        if (nBlockAvail == -1) return 0;
        while (position > BLOCK_SIZE)
        {
            BufferMapParent[nBlockAvail] = buffer->numberOfParent;
            position -= BLOCK_SIZE;
            nBlockAvail = GetAvailBlock();
        }
        uint8_t* addr = &iBuffer[nBlockAvail * BLOCK_SIZE];
        uint8_t* zAddr = addr;
        addr = addr + position;
        memcpy(addr, data, size);
        BufferMapParent[nBlockAvail] = buffer->numberOfParent;
        buffer->data = zAddr;
        nReqBlock--;
    }
    return 1;
}

bool Read_Buffer(t_buf* buffer, uint8_t* data, uint16_t size)
{
    if (size > BUFFER_SIZE) return 0;
    uint8_t* zeroAddr = data;
    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        if (BufferMapParent[i] == buffer->numberOfParent && size > 0)
        {
            uint8_t* b = &iBuffer[i * BLOCK_SIZE];
            uint16_t s;
            if (size > BLOCK_SIZE)
            {
                s = BLOCK_SIZE;
            }
            else
            {
                s = size;
            }
            memcpy(data, b, s);
            data += BLOCK_SIZE;
            size -= BLOCK_SIZE;
        }
    }
    data = zeroAddr;
    return 1;
}

bool Read_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
    if (size > BUFFER_SIZE) return 0;
    uint8_t* zeroAddr = data;
    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        if (BufferMapParent[i] == buffer->numberOfParent && size > 0)
        {
            uint8_t* b = &iBuffer[i * BLOCK_SIZE];
            uint16_t s;
            if (position > BLOCK_SIZE)
            {
                position -= BLOCK_SIZE;
                continue;
            }
            if (size > BLOCK_SIZE)
            {
                s = BLOCK_SIZE;
            }
            else
            {
                s = size;
            }
            b = b + position;
            memcpy(data, b, s);
            data += BLOCK_SIZE;
            size -= BLOCK_SIZE;
        }
    }
    data = zeroAddr;
    return 1;
}

bool Join_Buffer(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
    return 0;
}

bool Free_Buffer(t_buf* buffer)
{
    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        if (BufferMapParent[i] == buffer)
        {
            BufferMapParent[i] = 0;
        }
    }
    return 1;
}

bool Terminate_Buffer()
{
    free(iBuffer);
    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        BufferMapParent[i] = 0;
    }
    isInit = 0;
    return 1;
}

static void Init()
{
    iBuffer = (uint8_t*)calloc(sizeof(uint8_t), BUFFER_SIZE);
    isInit = 1;
}

static int8_t GetAvailBlock()
{
    for (size_t i = 0; i < BLOCK_COUNT; i++)
    {
        if (BufferMapParent[i] == 0)
        {
            return i;
        }
    }
    return -1;
}
