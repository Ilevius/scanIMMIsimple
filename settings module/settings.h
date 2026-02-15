#pragma once
#include <string>
#include "json.hpp"

using namespace std;





class Common_settings {
public:
	Common_settings() = default;

	const std::string& getWorkFolder() const { return work_folder_; }
	void setWorkFolder(const std::string& v) { work_folder_ = v; }


private:
	std::string work_folder_;
};

class Table_settings {
public:
	Table_settings() = default;

	const std::string& getIpAdress() const { return ip_adress_; }
	void setIpAdress(const std::string& v) { ip_adress_ = v; }

	bool getSimulator() const { return simulator_; }
	void setSimulator(bool v) { simulator_ = v; }

	bool getYAxis() const { return y_axis_; }
	void setYAxis(bool v) { y_axis_ = v; }

	int getXMin() const { return x_min_; }
	void setXMin(int v) { x_min_ = v; }

	int getYMin() const { return y_min_; }
	void setYMin(int v) { y_min_ = v; }

	int getXMax() const { return x_max_; }
	void setXMax(int v) { x_max_ = v; }

	int getYMax() const { return y_max_; }
	void setYMax(int v) { y_max_ = v; }


private:
	std::string ip_adress_;
	bool simulator_ = false;
	bool y_axis_ = false;
	int  x_min_ = 0;
	int  y_min_ = 0;
	int  x_max_ = 0;
	int  y_max_ = 0;
};

class Scan_settings {
public:
	Scan_settings() = default;

	double getLength() const { return length_; }
	void setLength(double v) { length_ = v; }



private:
	double length_ = 0.0;
};

class Config {
public:
	Config() = default;

	const Common_settings& getCommon_settings() const { return Common_settings_; }
	void setCommon(const Common_settings& c) { Common_settings_ = c; }

	const Table_settings& getTable_settings() const { return Table_settings_; }
	void setTable_settings(const Table_settings& t) { Table_settings_ = t; }

	const Scan_settings& getScan_settings() const { return Scan_settings_; }
	void setScan_settings(const Scan_settings& s) { Scan_settings_ = s; }



private:
	Common_settings Common_settings_;
	Table_settings  Table_settings_;
	Scan_settings   Scan_settings_;
};



// только объ€влени€ функций конверсии


	void to_json(nlohmann::json& j, const Config& c);
	void from_json(const nlohmann::json& j, Config& c);

	void to_json(nlohmann::json& j, const Scan_settings& s);
	void from_json(const nlohmann::json& j, Scan_settings& s);


	void to_json(nlohmann::json& j, const Table_settings& t);
	void from_json(const nlohmann::json& j, Table_settings& t);


	void to_json(nlohmann::json& j, const Common_settings& c);
	void from_json(const nlohmann::json& j, Common_settings& c);



//struct Config {
//	string IP_ADRESS = "";
//	bool SIMULATOR = true;
//	double BSCAN_LENGTH = 0.0;      
//	bool X_AXIS = false;        
//	string WORK_FOLDER = "wasn't read";   
//	void load(const string& filename = "settigngs.json");
//};
//
//extern Config SETTINGS;
//
//
//
