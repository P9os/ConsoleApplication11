#include "main.h"

#undef WRITEBUFFER_TEST
#undef WRITEBUFFERPOSITION_TEST
#undef READBUFFER_TEST
#undef READBUFFERPOSITION_TEST
#undef JOINBUFFER_TEST

int main(int argc, char** argv)
{
#ifdef WRITEBUFFER_TEST
	t_buf t11;
	Get_Buffer(&t11);
	Write_Buffer(&t11, (int8_t*)"First buffer\n", 14);
	t_buf t12;
	Get_Buffer(&t12);
	Write_Buffer(&t12, (int8_t*)"Second buffer\n", 15);

	for (size_t i = 0; i < 30; i++)
	{
		Write_Buffer(&t11, (int8_t*)"First buffer\n", 14);
		Write_Buffer(&t11, (char*)"First buffer\n", 14);
	}
#endif // WRITEBUFFER_TEST
#ifdef WRITEBUFFERPOSITION_TEST
	t_buf t21;
	Get_Buffer(&t21);
	Write_Buffer(&t21, (int8_t*)"Buffer", 7);
	Write_Buffer_Position(&t21, (int8_t*)"Position, write", 16, 6);
	t_buf t22;
	Get_Buffer(&t22);
	uint8_t data21[300];
	memset(&data21, 's', 300);
	Write_Buffer(&t22, "s", 2);
	t_buf t23;
	Get_Buffer(&t23);
	Write_Buffer(&t23, (int8_t*)"Third", 6);
	Write_Buffer(&t22, &data21, 300);
	Write_Buffer_Position(&t22, (int8_t*)"Hello", 6, 254);
#endif // WRITEBUFFERPOSITION_TEST
#ifdef READBUFFER_TEST
	t_buf t31;
	Get_Buffer(&t31);
	uint8_t data31[512];
	memset(data31, 's', 512);
	Write_Buffer(&t31, &data31, 512);
	memset(data31, 0, 512);
	Read_Buffer(&t31, &data31, 256);
#endif // READBUFFER_TEST
#ifdef READBUFFERPOSITION_TEST
	t_buf t41;
	Get_Buffer(&t41);
	uint8_t data41[512];
	memset(data41, 's', 512);
	Write_Buffer(&t41, &data41, 512);
	Write_Buffer_Position(&t41, (int8_t*)"It's position write", 20, 38);
	memset(data41, 0, 512);
	Read_Buffer_Position(&t41, &data41, 25, 38);
#endif // READBUFFERPOSITION_TEST
#ifdef JOINBUFFER_TEST
	t_buf t51;
	Get_Buffer(&t51);
	uint8_t data51[300];
	memset(data51, 0x50, 300);
	Write_Buffer(&t51, (int8_t*)"Stop", 5);
	t_buf t52;
	Get_Buffer(&t52);
	Write_Buffer(&t52, (int8_t*)"Hello", 6);
	Write_Buffer(&t51, &data51, 300);
	Join_Buffer(&t51, (int8_t*)"__join__", 9, 254);
#endif // JOINBUFFER_TEST

	Terminate_Buffer();
}
