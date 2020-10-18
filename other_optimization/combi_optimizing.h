//
// Created by Vova on 26.07.2020.
//

#pragma once

#include "GA/old_GA.h"
#include "local_optimization.h"

enum class combi_optimization_type {
	GA_and_GD,
	GA_and_GD_and_Newton
};

std::pair<double, std::vector<double>> combi_optimize(
		const std::function<double(const std::vector<double>&)>& error_function, // optimization_object,
		// TODO!!
		combi_optimization_type optimization_type
		);
