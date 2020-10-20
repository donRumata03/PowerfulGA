#pragma once
#include <vector>

double easom_function(const std::vector<double>& point);
double holder_function(const std::vector<double>& point);
double eggholder_function(const std::vector<double>& point); // For -512 <= x, y <= 512 : min = -959.6407 at (512, 404.2319)
double schaffer_function(const std::vector<double>& point); // For -100 <= x, y <= 100 : min = 0.292579 at (0, +- 1.25313)

double parabaloid(const std::vector<double>& point);
double parabaloid_fitness(std::vector<double>& xs);
inline double value_by_parabaloid_fitness(double fitness) { return 1 / fitness; }
std::vector<double> parabaloid_derivative(const std::vector<double>& point);
std::vector<double> parabaloid_second_derivative(const std::vector<double>& point);

double exponent(const std::vector < double >& point);

inline double easom_fit(const std::vector<double>& point) { return 1 / easom_function(point); }
inline double holder_fit(const std::vector<double>& point) { return 1 / (50 + holder_function(point)); }
inline double eggholder_fit(const std::vector<double>& point) { return 1 / (1000 + eggholder_function(point)); }

inline double shaffer_fit(const std::vector<double>& point) { return 1 / (schaffer_function(point) - 0.29); }
inline double value_by_shaffer_fit(double fit) { return 0.29 + 1 / fit; }


