#include "stdafx.h"
#include "math.h"

std::vector<double> generatePoints(double distance, int num_points) {
	std::vector<double> points(num_points);
	if (num_points <= 0) return points;

	double step = distance / (num_points - 1);
	for (int i = 0; i < num_points; i++) {
		points[i] = i * step;
	}
	return points;
}

std::vector<double> rawDataToVolts(const std::vector<int16_t>& raw_data,
	double x_zero, double v_div, int y_pos) {
	std::vector<double> volts(raw_data.size());
	for (size_t i = 0; i < raw_data.size(); i++) {
		double x = raw_data[i];							// сырой отсчёт
		volts[i] = (x-8192)/6400*v_div*2;				// Основная формула
	}
	return volts;
}