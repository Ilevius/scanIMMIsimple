#include <visa.h>
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
private:
	ViSession DEVICE;
	ViSession RESOURCE_MANAGER;
};