#include "stdafx.h"
#include <stdio.h>
#include <cstring>
#include "oscilloscope.h"
#include <visa.h>
#include <stdint.h> 
#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include "../settings module/settings.h"
using namespace std;

#pragma comment(lib, "visa64.lib")


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
	ask_and_print_answer("*IDN?\n");
	return 0;
}

int Oscilloscope::setup()
{
	auto& SETTINGS = Config::instance();


//											Here we setup the oscilloscope device
	string command = ":HORIzontal:SCALe "+SETTINGS.getOscill_settings().getHorScale()+"\n";
	viPrintf(DEVICE, command.c_str());
	Sleep(100);

	command = ":ACQuire:PRECision " + SETTINGS.getOscill_settings().getPrecision() + "\n";
	viPrintf(DEVICE, command.c_str());
	Sleep(100);

	command = ":"+SETTINGS.getOscill_settings().getDataCh() + ":DISPlay ON\n";
	viPrintf(DEVICE, command.c_str());
	Sleep(100);

	command = ":"+ SETTINGS.getOscill_settings().getDataCh() +":COUP " + SETTINGS.getOscill_settings().getDataCoup() + "\n";
	viPrintf(DEVICE, command.c_str());
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":TRIG:SING:MODE EDGE\n");
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":TRIG:SING:EDGE:COUP DC\n");
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":TRIG:SING:EDGE:SOUR CH2\n");
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":TRIG:SING:EDGE:LEV 1\n");
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":ACQ:DEPMEM 100k\n");
	Sleep(100);

	command = ":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n";
	viPrintf(DEVICE, ":CH1:SCAL 2v\n");
	Sleep(100);

//												Here check up settigs
	ask_and_print_answer(":HORIzontal:SCALe?\n");
	ask_and_print_answer(":ACQuire:PRECision?\n");
	ask_and_print_answer(":CH1:DISPlay?\n");
	ask_and_print_answer(":CH1:COUP?\n");
	ask_and_print_answer(":TRIG:SING:MODE?\n");
	ask_and_print_answer(":TRIG:SING:EDGE:COUP?\n");
	ask_and_print_answer(":TRIG:SING:EDGE:SOUR?\n");
	ask_and_print_answer(":TRIGger:SINGle:EDGE:LEVel?\n");
	ask_and_print_answer(":ACQ:DEPMEM?\n");
	ask_and_print_answer(":CH1:SCAL?\n");
	return 0;
}


int Oscilloscope::ask_and_print_answer(ViConstString inquire)
{
	char buffer[256];
	ViUInt32 bytes_read;
	ViStatus status;

	viPrintf(DEVICE, inquire);
	status = viRead(DEVICE, (ViBuf)buffer, 255, &bytes_read);

	if (status < VI_SUCCESS) {
		printf("Reading error: 0x%08X\n", status);
		return -1;
	}
	else {
		buffer[bytes_read] = 0;
		cout << "The ask is: " << inquire;
		printf(" The answer is: %s\n", buffer);
		return 0;
	}
}


int Oscilloscope::print_data()
{
	ViStatus status;
	// ----------------------------
	// 3. Запрос данных
	// ----------------------------
	const char* cmd = ":WAV:DATA?\n";
	viWrite(DEVICE, (ViBuf)cmd, strlen(cmd), NULL);

	// ----------------------------
	// 4. Читаем бинарный блок
	// ----------------------------
	unsigned char buffer[1024];
	ViUInt32 readCount = 0;

	status = viRead(DEVICE, buffer, sizeof(buffer), &readCount);

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

	for (int i = 0; i < 100; i++)
	{
		float volt = ((int8_t)data[i] - 128) * 0.039;  // 10V/256 уровней
		printf("%.2fV ", volt);
	}

	printf("\n");

	return 0;


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

}


int Oscilloscope::read_data()
{
	return 0;
}

size_t Oscilloscope::parseTMC(const char* buffer, size_t buf_len)
{
	if (buf_len < 2 || buffer[0] != '#') return 0;
	int n_digits = buffer[1] - '0';  // N из #N
	if (buf_len < 2 + n_digits) return 0;
	std::string len_str(buffer + 2, n_digits);
	return std::stoul(len_str);  // длина пакета
}

bool Oscilloscope::readRawWaveform(std::vector<int16_t>& waveform) {

	while (1) {
		char status[16];
		viQueryf(DEVICE, ":TRIGger:STATus?\n", "%s", status);
		if (strstr(status, "TRIG") || strstr(status, "TRIG")) break;  // Trigger Done
		Sleep(5);
	}


	ViUInt32 bytes_read;
	unsigned char read_buf[200099];
	// 1. Начать чтение
	viPrintf(DEVICE, ":WAV:BEG CH1\n");
	// 2. Задать offset и size  
	viPrintf(DEVICE, ":WAV:RANG 0,100000\n");
	// 3. Читать данные
	viPrintf(DEVICE, ":WAV:FETC?\n");

	viRead(DEVICE, read_buf, sizeof(read_buf) - 1, &bytes_read);
	read_buf[bytes_read] = '\0';

	// 4. Парсинг TMC + int16... (без изменений)

	if (bytes_read < 2 || read_buf[0] != '#') return false; // if answer contains less then 2 bytes or has no header
	int n_digits = read_buf[1] - '0';						// amount of digits in data bytes number
	if (bytes_read < 2 + n_digits) return false;			// if there is no data after header

	std::string len_str((char*)read_buf + 2, n_digits);		// the string of data bytes number
	size_t data_len = std::stoul(len_str);					// integer data bytes number

	waveform.clear();										// result int16 vector clear and set legth
	waveform.reserve(data_len / 2);

	// Извлечение int16 из байтов (big-endian)


	const unsigned char* data_ptr = read_buf + 2 + n_digits;

	for (size_t i = 0; i < data_len; i += 2) {
		uint16_t raw16 = (data_ptr[i+1] << 8) | (data_ptr[i] );  // 16-бит слово
		int16_t sample = (int16_t)(raw16 & 0x3FFF);	
		waveform.push_back(sample);
	}

	viPrintf(DEVICE, ":WAV:END\n");
	return waveform.size() == 100000;

}
