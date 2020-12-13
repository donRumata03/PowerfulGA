//
// Created by Vova on 09.12.2020.
//

#pragma once

#include <optimization_benchmarking/optimization_algorithm.h>
#include <Annealing/annealing_optimize.h>

class AnnealingSimulationAdapter : OptimizationAlgorithm
{
	explicit AnnealingSimulationAdapter(const AnnealingOptimizeParameters& params, const std::function<double(const std::vector<double>&)>& _energy_function);

	void set_computations (size_t computations) override;
	void set_exiting_value (double value) override;
	void run () override;
	bool success () override;
	std::vector<std::pair<size_t, double>> get_target_function_dynamic () override;

	// size_t iterations_planned {};
	AnnealingOptimizeParameters temp_params {};
	modificational_mutation mutator {};
	std::function<double(const std::vector<double>&)>& energy_function;

	AnnealingOptimizationOutput<double> result {};
};


