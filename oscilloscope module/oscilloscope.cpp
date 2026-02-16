#include "stdafx.h"
#include <stdio.h>
#include <cstring>
#include "oscilloscope.h"
#include <visa.h>
#include <stdint.h> 


#pragma comment(lib, "visa64.lib")
//
//
Oscilloscope::Oscilloscope()
{
	connect();
}

int Oscilloscope::connect() 
{
	//ViSession rm = VI_NULL;
	DEVICE = VI_NULL;
	RESOURCE_MANAGER = VI_NULL;
	ViStatus status;
	//
	// Открываем Resource Manager
	status = viOpenDefaultRM(&RESOURCE_MANAGER);
	if (status != VI_SUCCESS)
	{
		printf("RM error\n");
		return -1;
	}

	// Адрес прибора
	const char* resource =
	"USB0::0x5345::0x1235::2222099::INSTR";

	// Открываем прибор
	status = viOpen(RESOURCE_MANAGER, resource, VI_NULL, VI_NULL, &DEVICE);
	if (status != VI_SUCCESS)
	{
		printf("Device not found\n");
		return -1;
	}
	printf("Oscilloscope has been connected\n");
	return 0;

}


int Oscilloscope::disconnect()
{
	viClose(DEVICE);
	viClose(RESOURCE_MANAGER);
	printf("Oscilloscope has been disconnected\n");
	return 0;
}

int Oscilloscope::serial()
{
	char buffer[256];
	ViUInt32 bytes_read;
	ViStatus status;

	viPrintf(DEVICE, "*IDN?\n");
	status = viRead(DEVICE, (ViBuf)buffer, 255, &bytes_read);

	if (status < VI_SUCCESS) {
		printf("Reading error: 0x%08X\n", status);
	}
	else {
		buffer[bytes_read] = 0;
		printf("The answer is: %s\n", buffer);
	}
}





//// Таймаут
//viSetAttribute(scope, VI_ATTR_TMO_VALUE, 5000);

//// ----------------------------
//// 1. Выбираем канал
//// ----------------------------
//const char* src = ":MEASure:SOURce CH1\n";
//viWrite(scope, (ViBuf)src, strlen(src), NULL);

//// ----------------------------
//// 2. Формат BYTE (8 бит)
////// ----------------------------
//const char* fmt = ":ACQuire:PRECision 8\n";
//viWrite(scope, (ViBuf)fmt, strlen(fmt), NULL);

//// ----------------------------
//// 3. Запрос данных
//// ----------------------------
//const char* cmd = ":WAV:DATA?\n";
//viWrite(scope, (ViBuf)cmd, strlen(cmd), NULL);

//// ----------------------------
//// 4. Читаем бинарный блок
//// ----------------------------
//unsigned char buffer[1024];
//ViUInt32 readCount = 0;

//status = viRead(scope, buffer, sizeof(buffer), &readCount);

//if (status != VI_SUCCESS)
//{
//	printf("Read error\n");
//	return -1;
//}

//printf("Read %u bytes\n", readCount);

//// ----------------------------
//// 5. Пропускаем заголовок SCPI
//// ----------------------------
//// Формат: #<N><LEN><DATA>
//// Пример: #41000....

//int header_len = 0;

//if (buffer[0] == '#')
//{
//	int n = buffer[1] - '0';   // сколько цифр в длине
//	header_len = 2 + n;
//}

//unsigned char* data = buffer + header_len;

//// ----------------------------
//// 6. Печатаем первые 100 отсчётов
//// ----------------------------
//printf("First 1000 raw samples:\n");

//for (int i = 0; i < 100; i++)
//{
//	float volt = ((int8_t)data[i] - 128) * 0.039;  // 10V/256 уровней
//	printf("%.2fV ", volt);
//}

//printf("\n");

//printf("Press enter to close app");
//getchar();

//// ----------------------------
//// Закрываем
//// ----------------------------
//return 0;
