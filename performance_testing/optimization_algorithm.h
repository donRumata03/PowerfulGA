//
// Created by Vova on 07.12.2020.
//

#pragma once

#include <pythonic.h>

class OptimizationAlgorithm
{
	/// The algorithm should respond to this function by setting it's maximal number of computations to the argument value
	virtual void set_computations(size_t computations) = 0;

	/// Algorithm should satisfy and stop when it reaches this value
	virtual void set_exiting_value(double value) = 0;

	/// Evaluate the algorithm:
	virtual void run() = 0;

	/// The function should have stored the dynamic of the target function in time
	/// This function returns that dynamic
	virtual std::vector<std::pair<size_t, double>> get_target_function_dynamic() = 0;

	virtual ~OptimizationAlgorithm() = default;
};


