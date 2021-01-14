#ifndef __T_BUF_H_
#define __T_BUF_H_
#include "pch.h"

#define BLOCK_SIZE 256
#define BUFFER_SIZE 8192
#define BLOCK_COUNT (uint8_t)(BUFFER_SIZE/BLOCK_SIZE)

typedef struct
{
    uint16_t totalSize;
    int32_t numberOfParent;
    uint16_t offset;
} t_buf;

bool Get_Buffer(t_buf* buffer);

bool Write_Buffer(t_buf* buffer, uint8_t* data, uint16_t size);
bool Write_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);

bool Read_Buffer(t_buf* buffer, uint8_t* data, uint16_t size);
bool Read_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);

bool Join_Buffer(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);

bool Free_Buffer(t_buf* buffer);

bool Terminate_Buffer();
#endif // !__T_BUF_H_
