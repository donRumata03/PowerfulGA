#pragma once
#include <vector>

double easom_function(const std::vector<double>& point);
double holder_function(const std::vector<double>& point);
double eggholder_function(const std::vector<double>& point); // For -512 <= x, y <= 512 : min = -959.6407 at (512, 404.2319)

inline double schaffer_function(const std::vector<double>& point) // For -100 <= x, y <= 100 : min = 0.292579 at (0, +- 1.25313)
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



double parabaloid(const std::vector<double>& point);
double parabaloid_fitness(std::vector<double>& xs);
inline double value_by_parabaloid_fitness(double fitness) { return 1 / fitness; }
std::vector<double> parabaloid_derivative(const std::vector<double>& point);
std::vector<double> parabaloid_second_derivative(const std::vector<double>& point);

double exponent(const std::vector < double >& point);

inline double easom_fit(const std::vector<double>& point) { return 1 / easom_function(point); }
inline double holder_fit(const std::vector<double>& point) { return 1 / (50 + holder_function(point)); }
inline double eggholder_fit(const std::vector<double>& point) { return 1 / (1000 + eggholder_function(point)); }

inline double shaffer_fit(const std::vector<double>& point) { return 1 / (schaffer_function(point) - 0.); }
inline double value_by_shaffer_fit(double fit) { return 0. + 1 / fit; }



inline std::vector<double> schaffer_function_first_derivative(const std::vector<double>& point) {
	assert(point.size() == 2);
	double x = point[0], y = point[1];

	return {
			(x * (-0.004 * (square(cos(sin(abs(x * x - y * y)))) - 0.5) -
			      (4 * (x * x - y * y) * (0.001 * (x * x + y * y) + 1) * sin(sin(abs(x * x - y * y))) *
			       cos(abs(x * x - y * y)) * cos(sin(abs(x * x - y * y)))) / abs(square(x) - square(y))))
			/
			cube(0.001 * (square(x) + square(y)) + 1),


			0       // TODO
	};
}
