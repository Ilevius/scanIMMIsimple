#include "stdafx.h"
#include <stdio.h>
#include <cstring>
#include "oscilloscope.h"
#include <visa.h>
#include <stdint.h> 
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Windows.h>
#include <vector>
#include "../settings module/settings.h"
#include "../math module/math.h"
#include "../files module/files.h"
using namespace std;
#pragma comment(lib, "visa64.lib")


Oscilloscope::Oscilloscope()
{
	connect();
	Sleep(60);
	serial();
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
	auto& osc_sets = SETTINGS.getOscill_settings();

	string setup_commands[] = {
		":HORIzontal:SCALe " + SETTINGS.getOscill_settings().getHorScale() + "\n",  // horisontal scale
		":ACQuire:PRECision " + SETTINGS.getOscill_settings().getPrecision() + "\n",// precision
		":" + SETTINGS.getOscill_settings().getDataCh() + ":DISPlay ON\n",			// set chanell to display
		":" + SETTINGS.getOscill_settings().getDataCh() + ":OFFSet 1.2\n", 
		":" + SETTINGS.getOscill_settings().getDataCh() + ":COUP " + SETTINGS.getOscill_settings().getDataCoup() + "\n",
		":TRIG:SING:MODE "+ osc_sets.getTrigMode() +"\n",
		":TRIG:SING:EDGE:COUP "+osc_sets.getTrigCoup()+"\n",
		":TRIG:SING:EDGE:SOUR CH2\n",
		":TRIG:SING:EDGE:LEV "+osc_sets.getTrigEdgeLevel()+"\n",
		":ACQ:DEPMEM "+osc_sets.getDepmem()+"\n",
		":CH1:SCAL "+osc_sets.getDataChScale()+"\n"
	};


	//											Here we setup the oscilloscope device
	for (string command : setup_commands) {
		viPrintf(DEVICE, command.c_str());
		Sleep(100);
	}

	
//												Here check up settigs
	ask_and_print_answer(":HORIzontal:SCALe?\n");
	ask_and_print_answer(":ACQuire:PRECision?\n");
	ask_and_print_answer(":CH1:DISPlay?\n");
	ask_and_print_answer(":CH1:COUP?\n");
	ask_and_print_answer(":CH1:OFFSet?\n");
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

bool Oscilloscope::getRawDataSum(std::vector<int32_t>& waveform) {

	auto& SETTINGS = Config::instance();
	const int WANTED_TICKS = SETTINGS.getOscill_settings().getWantedTicks();
	const int AVE_NUM = SETTINGS.getScan_settings().getNave();
	std::vector<int32_t> waveform_sum(WANTED_TICKS, 0);

	const string WAV_RANGE = ":WAV:RANG 47000," + to_string(WANTED_TICKS) + "\n";

	ViUInt32 bytes_read;
	unsigned char read_buf[200100];
	char status[16];

	waveform.clear();										// result int16 vector clear and set legth
	waveform.resize(WANTED_TICKS, 0);

	for (int jj = 0; jj <AVE_NUM; jj++)
	{
		while (1) {
			viQueryf(DEVICE, ":TRIGger:STATus?\n", "%s", status);
			if (strstr(status, "TRIG") ) break;  // Trigger Done
			Sleep(5);
		}

		// 1. Начать чтение
		viPrintf(DEVICE, ":WAV:BEG CH1\n");
		// 2. Задать offset и size  
		viPrintf(DEVICE, WAV_RANGE.c_str());
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

		// Извлечение int16 из байтов (big-endian)
		const unsigned char* data_ptr = read_buf + 2 + n_digits;

		for (size_t i = 0; i < data_len; i += 2) {
			uint16_t raw16 = (data_ptr[i + 1] << 8) | (data_ptr[i]);  // 16-бит слово
			int16_t sample = (int16_t)(raw16 & 0x3FFF);
			waveform[i / 2] += sample;
		}
		viPrintf(DEVICE, ":WAV:END\n");
	}

	return waveform.size() == WANTED_TICKS;
}

bool Oscilloscope::voltsToFile(double x, double y)
{
	std::ostringstream oss;
	oss << "px"
		<< std::fixed << std::setprecision(2) << x
		<< "py"
		<< std::fixed << std::setprecision(2) << y
		<< ".txt";

	std::string filename = oss.str();


	std::vector<int32_t> my_data;
	bool res = getRawDataSum(my_data);
	std::vector<double> volts = rawDataToVolts(my_data, 5);
	saveWaveformToTxt(volts, filename);

	return true;
}



