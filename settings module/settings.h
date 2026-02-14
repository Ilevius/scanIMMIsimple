#pragma once
#include <string>
using namespace std;



struct Config {
	string IP_ADRESS = "";
	bool SIMULATOR = true;
	double BSCAN_LENGTH = 0.0;      
	bool X_AXIS = false;        
	string WORK_FOLDER = "wasn't read";   
	void load(const string& filename = "settigngs.json");
};

extern Config SETTINGS;