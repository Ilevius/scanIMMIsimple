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