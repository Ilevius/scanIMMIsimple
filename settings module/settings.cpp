#include "stdafx.h"
#include "settings.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

Config SETTINGS;

void Config::load(const std::string& filename)
{
	std::ifstream file("settings.json");
	json config = json::parse(file);
	IP_ADRESS = config["IP_ADRESS"];      // THE LENGTH OF BSCAN SEGMENT
	SIMULATOR = config["SIMULATOR"];      // THE LENGTH OF BSCAN SEGMENT
	BSCAN_LENGTH = config["BSCAN_LENGTH"];      // THE LENGTH OF BSCAN SEGMENT
	X_AXIS = config["X_AXIS"];        // true
	WORK_FOLDER = config["WORK_FOLDER"];   // "test"
}