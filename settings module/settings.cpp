#include "stdafx.h"
#include "settings.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

// Common
void to_json(json& j, const Common_settings& c) {
	j = json{
		{ "WORK_FOLDER", c.getWorkFolder() }
	};
}

void from_json(const json& j, Common_settings& c) {
	c.setWorkFolder(j.at("WORK_FOLDER").get<std::string>());
}

// Table
void to_json(json& j, const Table_settings& t) {
	j = json{
		{ "IP_ADRESS", t.getIpAdress() },
	{ "SIMULATOR", t.getSimulator() },
	{ "Y_AXIS",    t.getYAxis() },
	{ "X_MIN",     t.getXMin() },
	{ "Y_MIN",     t.getYMin() },
	{ "X_MAX",     t.getXMax() },
	{ "Y_MAX",     t.getYMax() }
	};
}

void from_json(const json& j, Table_settings& t) {
	t.setIpAdress(j.at("IP_ADRESS").get<std::string>());
	t.setSimulator(j.at("SIMULATOR").get<bool>());
	t.setYAxis(j.at("Y_AXIS").get<bool>());
	t.setXMin(j.at("X_MIN").get<int>());
	t.setYMin(j.at("Y_MIN").get<int>());
	t.setXMax(j.at("X_MAX").get<int>());
	t.setYMax(j.at("Y_MAX").get<int>());
}

// Scan
void to_json(json& j, const Scan_settings& s) {
	j = json{
		{ 
			"LENGTH", s.getLength(),
			"NPOINTS", s.getNpoints(),
			"AVE_NUM", s.getNave()
		}
	};
}

void from_json(const json& j, Scan_settings& s) {
	s.setLength(j.at("LENGTH").get<double>());
	s.setNpoints(j.at("NPOINTS").get<int>());
	s.setNave(j.at("AVE_NUM").get<int>());
}


// Oscilloscope
void to_json(json& j, const Oscill_settings& o) {
	j = json{
		{ 
			"HOR_SCALE", o.getHorScale(),
			"PRECISION", o.getPrecision(),
			"DATA_CH", o.getDataCh(),
			"TRIG_CH", o.getTrigCh(),
			"DATA_COUP", o.getDataCoup(),
			"TRIG_MODE", o.getTrigMode(),
			"TRIG_COUP", o.getTrigCoup(),
			"TRIG_EDGE_LEVEL", o.getTrigEdgeLevel(),
			"DATA_CH_SCALE", o.getDataChScale(),
			"DEPMEM", o.getDepmem(),
			"WANTED_TICKS", o.getWantedTicks(),
		}
	};
}

void from_json(const json& j, Oscill_settings& o) {
	o.setHorScale(j.at("HOR_SCALE").get<string>());
	o.setPrecision(j.at("PRECISION").get<string>());
	o.setDataCh(j.at("DATA_CH").get<string>());
	o.setTrigCh(j.at("TRIG_CH").get<string>());
	o.setDataCoup(j.at("DATA_COUP").get<string>());
	o.setTrigMode(j.at("TRIG_MODE").get<string>());
	o.setTrigCoup(j.at("TRIG_COUP").get<string>());
	o.setTrigEdgeLevel(j.at("TRIG_EDGE_LEVEL").get<string>());
	o.setDataChScale(j.at("DATA_CH_SCALE").get<string>());
	o.setDepmem(j.at("DEPMEM").get<string>());
	o.setWantedTicks(j.at("WANTED_TICKS").get<int>());
}

// Config (вложенные классы)
void to_json(json& j, const Config& c) {
	j = json{
		{ "COMMON", c.getCommon_settings() },
	{ "TABLE",  c.getTable_settings() },
	{ "SCAN",   c.getScan_settings() },
	{"OSCILLOSCOPE", c.getOscill_settings() }
	};
}

void from_json(const json& j, Config& c) {
	c.setCommon(j.at("COMMON").get<Common_settings>());
	c.setTable_settings(j.at("TABLE").get<Table_settings>());
	c.setScan_settings(j.at("SCAN").get<Scan_settings>());
	c.setOscill_settings(j.at("OSCILLOSCOPE").get<Oscill_settings>());
}


// Singleton
Config& Config::instance() {
	static Config cfg;
	static bool loaded = cfg.loadFromFile();  // автозагрузка
	return cfg;
}

// Загрузка
bool Config::loadFromFile(const std::string& filename) {
	std::ifstream in(filename);
	if (!in.is_open()) {
		return false;  // файл не найден — дефолты
	}

	try {
		json j;
		in >> j;
		from_json(j, *this);
		return true;
	}
	catch (...) {
		return false;
	}
}

// Сохранение
bool Config::saveToFile(const std::string& filename) {
	try {
		json j = *this;
		std::ofstream out(filename);
		out << j.dump(4);
		return true;
	}
	catch (...) {
		return false;
	}
}