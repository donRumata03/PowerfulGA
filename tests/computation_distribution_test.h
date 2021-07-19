//
// Created by Vova on 19.07.2021.
//

#pragma once

#include <GA/GA_optimizer.h>

inline void test_iteration_distribution() {
	double ep_pow = 0.3;
	size_t computations = 5000;

	auto distr = GA::distribute_computations_defaultly(computations, ep_pow);
	std::cout << distr << std::endl;

	
}
