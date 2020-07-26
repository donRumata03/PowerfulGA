#pragma once

#include "other_optimization/local_optimization.h"
#include "test_functions.h"

inline void test_gradient_counting()
{
	std::cout << get_second_gradient(exponent, { 1 }, 1e-5) << std::endl;
}

inline void test_GD()
{
	// std::cout << "GD: " << gradient_optimize(schaffer_function, { 0, -1.25 }, 0.01, 100);
	std::cout << "GD: " << gradient_optimize(parabaloid, parabaloid_derivative, { 0, -1.25 }, 0.1, 100,
			[](size_t iteration, const std::vector<double>& current_values ){
		std::cout << "After iteration " << iteration << ": argument value: " << current_values << ", function result = " << parabaloid(current_values) << std::endl;
	});
}

inline void test_newton_optimize()
{
	// std::cout << newton_optimize(schaffer_function, { 0, -1.25 }, 0.3, 100);

	std::cout << "Newton: " << newton_optimize(parabaloid, parabaloid_derivative, parabaloid_second_derivative, { 1, -1.25 }, 1, 1);
}


inline void compare_GD_and_newton()
{
	std::cout << "GD: " << gradient_optimize(parabaloid, { 1, -1.25 }, 0.5, 10) << std::endl;
	std::cout << "Newton: " << newton_optimize(parabaloid, { 1, -1.25 }, 1, 10) << std::endl;
}
