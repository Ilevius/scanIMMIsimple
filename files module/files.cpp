#include "stdafx.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include "files.h"

void saveWaveformToTxt(const std::vector<double>& waveform, const std::string& filename) {
	std::ofstream file(filename);
	file << std::fixed << std::setprecision(6);  // 6 знаков после запятой

	for (double v : waveform) {
		file << v << '\n';
	}
	file.close();
}