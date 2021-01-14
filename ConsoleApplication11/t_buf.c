#include "t_buf.h"

static uint8_t isInit = 0;
static uint8_t* iBuffer;
static int32_t BufferMapParent[BLOCK_COUNT] = { 0 };

static void Init();
static int8_t GetAvailBlock();

bool Get_Buffer(t_buf* buffer)
{
	if (!isInit)
	{
		Init();
	}
	buffer->totalSize = BUFFER_SIZE;
	buffer->numberOfParent = rand();
	buffer->offset = 0;
	return 1;
}

bool Write_Buffer(t_buf* buffer, uint8_t* data, uint16_t size)
{
	if (size > BUFFER_SIZE || size == 0 || data == NULL) return 0;
	uint8_t nReqBlocks = size / BLOCK_SIZE;
	nReqBlocks += 1;
	while (nReqBlocks && size < BUFFER_SIZE)
	{
		int8_t nBlockAvail = GetAvailBlock();
		if (nBlockAvail == -1) return 0;
		int16_t s;
		s = (size > BLOCK_SIZE) ? BLOCK_SIZE : size;
		uint8_t* addr = &iBuffer[nBlockAvail * BLOCK_SIZE];
		memcpy(addr, data, s);
		BufferMapParent[nBlockAvail] = buffer->numberOfParent;
		size -= BLOCK_SIZE;
		nReqBlocks--;
	}
	return 1;
}

bool Write_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
	if (size > BUFFER_SIZE || size == 0 || data == NULL) return 0;
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
		if (BufferMapParent[i] == buffer->numberOfParent && size > 0 && size < BUFFER_SIZE)
		{
			uint8_t* b = &iBuffer[i * BLOCK_SIZE];
			uint16_t s;
			s = (size > BLOCK_SIZE) ? BLOCK_SIZE : size;
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
		if (BufferMapParent[i] == buffer->numberOfParent)
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

static inline void Init()
{
	iBuffer = (uint8_t*)calloc(sizeof(uint8_t), BUFFER_SIZE);
	isInit = 1;
}

static inline int8_t GetAvailBlock()
{
	for (int8_t i = 0; i < BLOCK_COUNT; i++)
	{
		if (BufferMapParent[i] == 0)
		{
			return i;
		}
	}
	return -1;
}
