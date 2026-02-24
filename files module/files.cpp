
#include "stdafx.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include "files.h"
#include <algorithm>
#include "mat.h"  // MATLAB Engine MAT API

void saveWaveformToTxt(const std::vector<double>& waveform, const std::string& filename) {
	std::ofstream file(filename);
	file << std::fixed << std::setprecision(6);  // 6 знаков после запятой

	for (double v : waveform) {
		file << v << '\n';
	}
	file.close();
}


bool createBscanMat(const std::vector<std::vector<double>>& data,
	const std::vector<double>& coord_,
	const std::vector<double>& time_,
	const std::string& filename) {

	MATFile* matfp = matOpen(filename.c_str(), "w");
	if (!matfp) {
		return false;
	}

	int Nx = data.size();
	int Nt = (Nx > 0) ? data[0].size() : 0;

	// Проверка пустых данных
	if (Nx == 0 || Nt == 0) {
		matClose(matfp);
		return false;
	}

	// 1. data (Nx x Nt)
	mxArray* mx_data = mxCreateDoubleMatrix(Nx, Nt, mxREAL);
	double* data_ptr = mxGetPr(mx_data);
	for (int i = 0; i < Nx; ++i) {
		for (int j = 0; j < Nt; ++j) {
			data_ptr[i * Nt + j] = data[i][j];
		}
	}
	matPutVariable(matfp, "data", mx_data);
	mxDestroyArray(mx_data);

	// 2. data_norm (Nx x Nt) — нормализованные данные
	mxArray* mx_data_norm = mxCreateDoubleMatrix(Nx, Nt, mxREAL);
	double* norm_ptr = mxGetPr(mx_data_norm);
	for (int i = 0; i < Nx; ++i) {
		double max_val = data[i][0];
		double min_val = data[i][0];

		// Поиск max/min
		for (int j = 0; j < Nt; ++j) {
			if (data[i][j] > max_val) max_val = data[i][j];
			if (data[i][j] < min_val) min_val = data[i][j];
		}

		double amp = (max_val - min_val) / 2.0;
		if (amp == 0.0) amp = 1.0;  //  Защита от деления на ноль!

		for (int j = 0; j < Nt; ++j) {
			norm_ptr[i * Nt + j] = data[i][j] / amp;
		}
	}
	matPutVariable(matfp, "data_norm", mx_data_norm);
	mxDestroyArray(mx_data_norm);

	// 3. coord_ (1 x Nx)
	mxArray* mx_coord = mxCreateDoubleMatrix(1, coord_.size(), mxREAL);
	double* coord_ptr = mxGetPr(mx_coord);
	for (size_t i = 0; i < coord_.size(); ++i) {
		coord_ptr[i] = coord_[i];
	}
	matPutVariable(matfp, "coord_", mx_coord);
	mxDestroyArray(mx_coord);

	// 4. time_ (1 x Nt)
	mxArray* mx_time = mxCreateDoubleMatrix(1, time_.size(), mxREAL);
	double* time_ptr = mxGetPr(mx_time);
	for (size_t i = 0; i < time_.size(); ++i) {
		time_ptr[i] = time_[i];
	}
	matPutVariable(matfp, "time_", mx_time);
	mxDestroyArray(mx_time);

	matClose(matfp);
	return true;
}
