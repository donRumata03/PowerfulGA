//
// Created by Vova on 17.09.2020.
//

#pragma once

#include <pythonic.h>

/**
 * It`s already configured!
 * Takes std::function<std::vector<double>>
 * Returns std::pair<double, std::vector<double>>, which is { best_value, best_genome }
 *
 * by the given precision and maximal iteration number, it returns the number of iterations required to get it or that maximal iteration number
 */
using optimization_algorithm_t = std::function<size_t(std::vector<double>)>; // std::function<std::pair<double, std::vector<double>>()>; // TODO!


/**
 * @returns: The non-smoothed distribution graph
 */
std::vector<std::pair<double, double>> make_optimization_algorithm_performance_distribution();



