#include "t_buf.h"

static uint8_t isInit = 0;
static uint8_t* iBuffer;
static int32_t BufferMapParent[BLOCK_COUNT] = { 0 };

static void Init(void);
static int8_t FindBlock(int32_t numberOfParent, uint8_t countSkip);
static uint8_t* GetAddr(int32_t numberOfParentToFind, uint16_t offset);
static uint8_t* GetAddrAndMarkBlock(int32_t numberOfParentToFind, int32_t numberForMark, uint16_t offset);
static uint8_t ShiftBufferLeft(t_buf* buffer, uint16_t position, uint16_t CountByteForShift);

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
	uint16_t o = buffer->offset;
	uint8_t* addr = GetAddrAndMarkBlock(buffer->numberOfParent, buffer->numberOfParent, o);
	if (addr == NULL)
	{
		addr = GetAddrAndMarkBlock(0, buffer->numberOfParent, o);
		if (addr == NULL)
		{
			return 0;
		}
	}
	addr += o % BLOCK_SIZE;
	while (size)
	{
		if (o % BLOCK_SIZE == 0 && o != 0 )
		{
			// alloc new block
			addr = GetAddrAndMarkBlock(0, buffer->numberOfParent, 0);
			if (addr == NULL) return 0;
		}
		*addr++ = *data++;
		size--;
		o++;
	}
	buffer->offset = o;
	return 1;
}

bool Write_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
	if (size > BUFFER_SIZE || size == 0 || data == NULL || position > buffer->offset) return 0;
	uint16_t o = buffer->offset;
	uint8_t* addr = GetAddrAndMarkBlock(buffer->numberOfParent, buffer->numberOfParent, position);
	if (addr == NULL) return 0;
	addr += position % BLOCK_SIZE;
	while (size)
	{
		if (position % BLOCK_SIZE == 0 && o != 0)
		{
			addr = GetAddrAndMarkBlock(buffer->numberOfParent, buffer->numberOfParent, position);
			if (addr == NULL)
			{
				addr = GetAddrAndMarkBlock(0, buffer->numberOfParent, 0);
				if (addr == NULL) return 0;
			}
		}
		*addr++ = *data++;
		position++;
		if (position == o) o++;
		size--;
	}
	buffer->offset = o;
	return 1;
}

bool Read_Buffer(t_buf* buffer, uint8_t* data, uint16_t size)
{
	if (size > BUFFER_SIZE) return 0;
	uint16_t o = buffer->offset;
	uint16_t p = 0;
	while (size)
	{
		uint8_t* addr = GetAddr(buffer->numberOfParent, p);
		addr += p % BLOCK_SIZE;
		*data = *addr;
		data++;
		p++;
		size--;
		if (p == o) return 1;
	}
	return 1;
}

bool Read_Buffer_Position(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
	if (size > BUFFER_SIZE) return 0;
	uint16_t o = buffer->offset;
	uint16_t p = position;
	while (size)
	{
		uint8_t* addr = GetAddr(buffer->numberOfParent, p);
		addr += p % BLOCK_SIZE;
		*data = *addr;
		data++;
		p++;
		size--;
		if (p == o) return 1;
	}
	return 1;
}

bool Join_Buffer(t_buf* buffer, uint8_t* data, uint16_t size, uint16_t position)
{
	if (size > BUFFER_SIZE || data == NULL || size == 0 || position > buffer->offset) return 0;
	if (ShiftBufferLeft(buffer, position, size))
	{
		Write_Buffer_Position(buffer, data, size, position);
	}
	else
	{
		return 0;
	}
	return 1;
}

bool Free_Buffer(t_buf* buffer)
{
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		if (BufferMapParent[i] == buffer->numberOfParent)
		{
			uint8_t* addr = &iBuffer[i * BLOCK_SIZE];
			memset(addr, 0x00, 256);
			BufferMapParent[i] = 0;
		}
	}
	return 1;
}

bool Terminate_Buffer(void)
{
	free(iBuffer);
	for (size_t i = 0; i < BLOCK_COUNT; i++)
	{
		BufferMapParent[i] = 0;
	}
	isInit = 0;
	return 1;
}

static inline void Init(void)
{
	iBuffer = (uint8_t*)calloc(sizeof(uint8_t), BUFFER_SIZE);
	isInit = 1;
}

static inline int8_t FindBlock(int32_t numberOfParent, uint8_t countSkip)
{
	for (int8_t i = 0; i < BLOCK_COUNT; i++)
	{
		if (BufferMapParent[i] == numberOfParent)
		{
			if (countSkip)
			{
				countSkip--;
				continue;
			}
			else
			{
				return i;
			}
		}
	}
	return NOT_FOUND;
}

static inline uint8_t* GetAddr(int32_t numberOfParentToFind, uint16_t offset)
{
	uint8_t nBlock = FindBlock(numberOfParentToFind, (uint8_t)(offset / BLOCK_SIZE));
	return (nBlock != NOT_FOUND) ? &iBuffer[nBlock * BLOCK_SIZE] : NULL;
}

static inline uint8_t* GetAddrAndMarkBlock(int32_t numberOfParentToFind, int32_t numberForMark, uint16_t offset)
{
	int8_t nBlock = FindBlock(numberOfParentToFind, (uint8_t)(offset / BLOCK_SIZE));
	if (nBlock != NOT_FOUND)
	{
		BufferMapParent[nBlock] = numberForMark;
		return &iBuffer[nBlock * BLOCK_SIZE];
	}
	else
	{
		return NULL;
	}
}

static inline uint8_t ShiftBufferLeft(t_buf* buffer, uint16_t position, uint16_t CountByteForShift)
{
	uint16_t s = buffer->offset - position;
	uint16_t p = position;
	uint8_t b = 0;
	t_buf tempBuf;
	Get_Buffer(&tempBuf);
	uint8_t tempData;
	while (s)
	{
		b = Read_Buffer_Position(buffer, &tempData, 1, p);
		b = Write_Buffer(&tempBuf, &tempData, 1);
		p++;
		s--;
	}
	s = buffer->offset - position;
	p = position + CountByteForShift;
	uint16_t i = 0;
	while (s)
	{
		b = Read_Buffer_Position(&tempBuf, &tempData, 1, i);
		b = Write_Buffer_Position(buffer, &tempData, 1, p);
		p++;
		i++;
		s--;
	}
	Free_Buffer(&tempBuf);
	return b;
}
