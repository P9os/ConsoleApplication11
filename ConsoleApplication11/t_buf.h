#ifndef __T_BUF_H_
#define __T_BUF_H_
#include "pch.h"

#define BLOCK_SIZE 256
#define BUFFER_SIZE 8192
#define BLOCK_COUNT (uint8_t)(BUFFER_SIZE/BLOCK_SIZE)

#define NOT_FOUND -1
#pragma pack(push, 1)
typedef struct
{
    uint16_t totalSize;
    int32_t numberOfParent;
    uint16_t offset;
} t_buf;
#pragma pack(pop)
/// <summary>
/// Initialize new buffer
/// </summary>
/// <param name="buffer">Link on unready buffer</param>
/// <returns>Success</returns>
bool Get_Buffer(t_buf* buffer);
/// <summary>
/// Write to buffer
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <returns>Success</returns>
bool Write_Buffer(t_buf* buffer, uint8_t* data, uint16_t size);
/// <summary>
/// Write to buffer with position number
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <param name="position">Shift in allocated buffer</param>
/// <returns>Success</returns>
bool Write_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);
/// <summary>
/// Read from internal buffer
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <returns></returns>
bool Read_Buffer(t_buf* buffer, uint8_t* data, uint16_t size);
/// <summary>
/// Read from internal buffer with position number
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <param name="position">Shift in allocated buffer</param>
/// <returns>Success</returns>
bool Read_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);
/// <summary>
/// Join into the buffer
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <param name="position">Shift in allocated buffer</param>
/// <returns>Success</returns>
bool Join_Buffer(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position);

bool Free_Buffer(t_buf* buffer);
/// <summary>
/// Terminate Buffer
/// </summary>
/// <param name=""></param>
/// <returns>Success</returns>
bool Terminate_Buffer(void);
#endif // !__T_BUF_H_
