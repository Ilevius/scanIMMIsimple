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
	double getNpoints() const { return NPOINTS_; }
	void setNpoints(int v) { NPOINTS_ = v; }



private:
	double length_ = 0.0;
	int NPOINTS_ = 0;
};

class Oscill_settings {
public:
	Oscill_settings() = default;

	const string getHorScale() const { return horScale_; }
	void setHorScale(const std::string& v) { horScale_ = v; }

	const string getPrecision() const { return precision_; }
	void setPrecision(const std::string& v) { precision_ = v; }

	const string getDataCh() const { return dataCh_; }
	void setDataCh(const std::string& v) { dataCh_ = v; }

	const string getTrigCh() const { return trigCh_; }
	void setTrigCh(const std::string& v) { trigCh_ = v; }

	const string getDataCoup() const { return dataCoup_; }
	void setDataCoup(const std::string& v) { dataCoup_ = v; }

	const string getTrigMode() const { return trigMode_; }
	void setTrigMode(const std::string& v) { trigMode_ = v; }

	const string getTrigCoup() const { return trigCoup_; }
	void setTrigCoup(const std::string& v) { trigCoup_ = v; }

	const string getTrigEdgeLevel() const { return trigEdgeLevel_; }
	void setTrigEdgeLevel(const std::string& v) { trigEdgeLevel_ = v; }

	const string getDataChScale() const { return dataChScale_; }
	void setDataChScale(const std::string& v) { dataChScale_ = v; }

	const string getDepmem() const { return depmem_; }
	void setDepmem(const std::string& v) { depmem_ = v; }

	const int getWantedTicks() const { return wantedTicks_; }
	void setWantedTicks(const int v) { wantedTicks_ = v; }

private:
	string horScale_ = "";
	string precision_ = "";	
	string dataCh_ = "";
	string trigCh_ = "";	
	string dataCoup_ = "";
	string trigMode_ = "";	
	string trigCoup_ = "";
	string trigEdgeLevel_ = "";	
	string dataChScale_ = "";
	string depmem_ = "";
	int wantedTicks_ = 20000;
};

class Config {
public:
	static Config& instance();

	bool loadFromFile(const std::string& filename = "settings.json");
	bool saveToFile(const std::string& filename = "settings.json");

	const Common_settings& getCommon_settings() const { return Common_settings_; }
	void setCommon(const Common_settings& c) { Common_settings_ = c; }

	const Table_settings& getTable_settings() const { return Table_settings_; }
	void setTable_settings(const Table_settings& t) { Table_settings_ = t; }

	const Scan_settings& getScan_settings() const { return Scan_settings_; }
	void setScan_settings(const Scan_settings& s) { Scan_settings_ = s; }

	const Oscill_settings& getOscill_settings() const { return Oscill_settings_; }
	void setOscill_settings(const Oscill_settings& o) { Oscill_settings_ = o; }

private:
	Config() = default;
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	Common_settings Common_settings_;
	Table_settings  Table_settings_;
	Scan_settings   Scan_settings_;
	Oscill_settings Oscill_settings_;
};


// только объ€влени€ функций конверсии
	void to_json(nlohmann::json& j, const Config& c);
	void from_json(const nlohmann::json& j, Config& c);

	void to_json(nlohmann::json& j, const Scan_settings& s);
	void from_json(const nlohmann::json& j, Scan_settings& s);

	void to_json(nlohmann::json& j, const Table_settings& t);
	void from_json(const nlohmann::json& j, Table_settings& t);

	void to_json(nlohmann::json& j, const Oscill_settings& o);
	void from_json(const nlohmann::json& j, Oscill_settings& o);

	void to_json(nlohmann::json& j, const Common_settings& c);
	void from_json(const nlohmann::json& j, Common_settings& c);