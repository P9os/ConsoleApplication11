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
	int nReqBlocks = size / BLOCK_SIZE;
	if (nReqBlocks == 0)
	{
		int nBlockAvail = GetAvailBlock();
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
			int nBlockAvail = GetAvailBlock();
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

}

bool Read_Buffer(t_buf* buffer, uint8_t* data, uint16_t size)
{
	if (size > BUFFER_SIZE || !(size%256==0)) return 0;
	uint8_t* zeroAddr = data;
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		if (BufferMapParent[i] == buffer->numberOfParent && size > 0)
		{
			uint8_t* b = &iBuffer[i * BLOCK_SIZE];
			memcpy(data, b, BLOCK_SIZE);
			data += BLOCK_SIZE;
			size -= BLOCK_SIZE;
		}
	}
	data = zeroAddr;
	return 1;
}

bool Read_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{

}

bool Join_Buffer(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{

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
