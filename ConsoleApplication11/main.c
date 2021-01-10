#include "main.h"

int main(int argc, char** argv)
{
	t_buf t1, t2, t3;
	Get_Buffer(&t1);
	Write_Buffer(&t1, (uint8_t*)"First buffer", 13);
	Get_Buffer(&t2);
	Write_Buffer(&t2, (uint8_t*)"Second buffer", 14);
	Get_Buffer(&t3);
	Write_Buffer(&t3, (uint8_t*)"Triple buffer", 14);

	Write_Buffer(&t2, "Yet Second buffer", 18);

	uint8_t data4[512];
	for (size_t i = 0; i < sizeof(data4); i++)
	{
		data4[i] = 's';
		if (i == 511)
		{
			data4[i] = '\0';
		}
	}
	Write_Buffer(&t1, &data4, 512);

	uint8_t data1[256];
	Read_Buffer(&t1, &data1, 256);
	puts(data1);
	uint8_t data2[512];
	Read_Buffer(&t2, &data2, 512);
	puts(data2);
	uint8_t data3[256];
	Read_Buffer(&t3, &data3, 256);
	puts(data3);

	uint8_t data5[1024];
	Read_Buffer(&t1, &data5, 1024);

	Terminate_Buffer();
}
