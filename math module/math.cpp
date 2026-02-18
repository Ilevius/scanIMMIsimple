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
		double x = raw_data[i];  // сырой отсчёт

								 // Основная формула
								 volts[i] = (x-4000)*10*v_div/16384;
		//volts[i] = (x - x_zero) * v_div * pow(10, -y_pos);
	}

	return volts;
}