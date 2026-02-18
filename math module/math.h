#pragma once
#include <vector>

std::vector<double> generatePoints(double distance, int num_points);
std::vector<double> rawDataToVolts(const std::vector<int16_t>& raw_data, double x_zero, double v_div, int y_pos);
