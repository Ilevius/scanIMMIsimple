#include <vector>
#pragma once

void saveWaveformToTxt(const std::vector<double>& waveform, const std::string& filename);

bool createBscanMat(const std::vector<std::vector<double>>& data,      // Nx x Nt (Nx строк замеров)
	const std::vector<double>& coord_,                  // координаты X (Nx элементов)  
	const std::vector<double>& time_,                   // время (Nt элементов)
	const double dist_step_,							// длинна вектора между каждой парой последовательных точек
	const double time_step_,
	const std::string& filename);

