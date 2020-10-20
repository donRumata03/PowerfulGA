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
	return -abs(sin(x) * cos(y) * exp(abs(1 - (sqrt(square(x) + square(y))) / (pi))));
}

double eggholder_function(const std::vector<double>& point)
{
	assert(point.size() == 2);
	double x = point[0], y = point[1];
	return -(y + 47) * sin(sqrt(abs(x / 2 + (y + 47)))) - x * sin(sqrt(abs(x - (y + 47))));
}

double schaffer_function(const std::vector<double>& point)
{
	if (point.size() != 2) {
		std::cout << console_colors::red /*<< console_colors::bold*/ << console_colors::underlined << "The size of genome "  << point
			<< " is " << point.size() << ", but should be " << 2 << console_colors::remove_all_colors
		<< std::endl;
	}

	assert(point.size() == 2);
	double x = point[0], y = point[1];
	return 0.5 + (square(cos(sin(abs(square(x) - square(y))))) - 0.5) / square(1 + 0.001 * (square(x) + square(y)));
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


