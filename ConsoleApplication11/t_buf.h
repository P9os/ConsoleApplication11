#ifndef __T_BUF_H_
#define __T_BUF_H_
#include "pch.h"

#define BLOCK_SIZE 256
#define BLOCK_COUNT 32
#define BUFFER_SIZE 8192

typedef struct
{
    uint8_t* data;
    uint16_t totalSize;
    int32_t numberOfParent;
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
