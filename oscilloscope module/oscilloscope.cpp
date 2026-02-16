#include "stdafx.h"
#include <stdio.h>
#include <cstring>
#include "oscilloscope.h"
#include <visa.h>


#pragma comment(lib, "visa64.lib")
//
//
Oscilloscope::Oscilloscope()
{
	connect();
}

int Oscilloscope::connect() 
{
	ViSession rm = VI_NULL;
	ViSession scope = VI_NULL;
	ViStatus status;
	//
	// Открываем Resource Manager
	status = viOpenDefaultRM(&rm);
	if (status != VI_SUCCESS)
	{
		printf("RM error\n");
		return -1;
	}

	// Адрес прибора
	const char* resource =
	"USB0::0x5345::0x1235::2222099::INSTR";

	// Открываем прибор
	status = viOpen(rm, resource, VI_NULL, VI_NULL, &scope);
	if (status != VI_SUCCESS)
	{
		printf("Device not found\n");
		return -1;
	}

	// Таймаут
	viSetAttribute(scope, VI_ATTR_TMO_VALUE, 5000);

	// ----------------------------
	// 1. Выбираем канал
	// ----------------------------
	const char* src = ":MEASure:SOURce CH1\n";
	viWrite(scope, (ViBuf)src, strlen(src), NULL);

	// ----------------------------
	// 2. Формат BYTE (8 бит)
	//// ----------------------------
	const char* fmt = ":ACQuire:PRECision 12\n";
	viWrite(scope, (ViBuf)fmt, strlen(fmt), NULL);

	// ----------------------------
	// 3. Запрос данных
	// ----------------------------
	const char* cmd = ":WAV:DATA?\n";
	viWrite(scope, (ViBuf)cmd, strlen(cmd), NULL);

	// ----------------------------
	// 4. Читаем бинарный блок
	// ----------------------------
	unsigned char buffer[1024];
	ViUInt32 readCount = 0;

	status = viRead(scope, buffer, sizeof(buffer), &readCount);

	if (status != VI_SUCCESS)
	{
		printf("Read error\n");
		return -1;
	}

	printf("Read %u bytes\n", readCount);

	// ----------------------------
	// 5. Пропускаем заголовок SCPI
	// ----------------------------
	// Формат: #<N><LEN><DATA>
	// Пример: #41000....

	int header_len = 0;

	if (buffer[0] == '#')
	{
		int n = buffer[1] - '0';   // сколько цифр в длине
		header_len = 2 + n;
	}

	unsigned char* data = buffer + header_len;

	// ----------------------------
	// 6. Печатаем первые 100 отсчётов
	// ----------------------------
	printf("First 1000 raw samples:\n");

	for (int i = 0; i < 1000; i++)
	{
		printf("%3d ", data[i]);
	}

	printf("\n");

	printf("Press enter to close app");
	getchar();

	// ----------------------------
	// Закрываем
	// ----------------------------
	viClose(scope);
	viClose(rm);
	return 0;
}