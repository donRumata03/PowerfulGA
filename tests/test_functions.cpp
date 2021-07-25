#include "test_functions.h"
#include <cassert>
#include <cmath>

#include <pythonic.h>

double easom_function(const std::vector<double>& point)
{
	assert(point.size() == 2);
	double x = point[0], y = point[1];
	return -cos(x) * cos(y) * exp(-(square(x - pi) + square(y - pi)));
}

double holder_function(const std::vector<double>& point)
{
	assert(point.size() == 2);
	double x = point[0], y = point[1];
	return -std::abs(sin(x) * cos(y) * exp(std::abs(1 - (sqrt(square(x) + square(y))) / (pi))));
}

double eggholder_function(const std::vector<double>& point)
{
	assert(point.size() == 2);
	double x = point[0], y = point[1];
	return -(y + 47) * sin(sqrt(std::abs(x / 2 + (y + 47)))) - x * sin(sqrt(std::abs(x - (y + 47))));
}


double parabaloid(const std::vector<double>& point)
{
	double accum = 0;

	for (auto& val : point) accum += val * val;
	
	return accum;
}

double exponent(const std::vector<double>& point)
{
	return exp(point[0]);
}

std::vector<double> parabaloid_derivative (const std::vector<double> &point)
{
	auto res = point;

	for (auto& v : res) {
		v *= 2;
	}

	return res;
}

std::vector<double> parabaloid_second_derivative (const std::vector<double> &point)
{
	std::vector<double> res(point.size(), 2);
	return res;
}

double parabaloid_fitness (std::vector<double> &xs)
{
	return 1 / parabaloid(xs);
}


