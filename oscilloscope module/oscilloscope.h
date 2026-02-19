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
	bool readRawWaveform(std::vector<int32_t>& waveform);
private:
	ViSession DEVICE;
	ViSession RESOURCE_MANAGER;
};