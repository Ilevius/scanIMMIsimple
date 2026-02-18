#include <visa.h>
#include <vector>
#pragma once
#pragma comment(lib, "visa64.lib")
class Oscilloscope
{
public:
	Oscilloscope();
	int disconnect();
	int connect();
	int serial();
	int setup();
	int ask_and_print_answer(ViConstString inquiry);
	int print_data();

	int read_data();
	size_t parseTMC(const char* buffer, size_t buf_len);
	bool readRawWaveform(std::vector<int16_t>& waveform);
private:
	ViSession DEVICE;
	ViSession RESOURCE_MANAGER;
};