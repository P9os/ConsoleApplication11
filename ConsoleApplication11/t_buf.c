#include "t_buf.h"

static uint8_t isInit = 0;
static uint8_t* iBuffer;
static int32_t BufferMapParent[BLOCK_COUNT] = { 0 };

static void Init(void);
static int8_t FindBlock(int32_t numberOfParent, uint8_t countSkip);
static uint8_t* GetAddr(int32_t numberOfParentToFind, uint16_t offset);
static uint8_t* GetAddrAndMarkBlock(int32_t numberOfParentToFind, int32_t numberForMark, uint16_t offset);
/// <summary>
/// Initialize new buffer
/// </summary>
/// <param name="buffer">Link on unready buffer</param>
/// <returns>Success</returns>
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
/// <summary>
/// Write to buffer
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <returns>Success</returns>
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
/// <summary>
/// Write to buffer with position number
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <param name="position">Shift in allocated buffer</param>
/// <returns>Success</returns>
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
/// <summary>
/// Read from internal buffer
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <returns></returns>
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
/// <summary>
/// Read from internal buffer with position number
/// </summary>
/// <param name="buffer">Link on initialized buffer</param>
/// <param name="data">Link on copyiable buffer</param>
/// <param name="size">Size of data buffer</param>
/// <param name="position">Shift in allocated buffer</param>
/// <returns>Success</returns>
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
	uint8_t nBlock = FindBlock(numberOfParentToFind, offset / BLOCK_SIZE);
	return (nBlock != NOT_FOUND) ? &iBuffer[nBlock * BLOCK_SIZE] : NULL;
}

static inline uint8_t* GetAddrAndMarkBlock(int32_t numberOfParentToFind, int32_t numberForMark, uint16_t offset)
{
	int8_t nBlock = FindBlock(numberOfParentToFind, offset / BLOCK_SIZE);
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
