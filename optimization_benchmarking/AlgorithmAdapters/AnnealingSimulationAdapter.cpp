//
// Created by Vova on 09.12.2020.
//

#include "AnnealingSimulationAdapter.h"


AnnealingSimulationAdapter::AnnealingSimulationAdapter (const AnnealingOptimizeParameters& params,
														const std::function<double(const std::vector<double>&)>& _energy_function)
		: temp_params(params), energy_function(_energy_function) {

}

void AnnealingSimulationAdapter::set_computations (size_t computations)
{
	temp_params.iterations = computations;
}

void AnnealingSimulationAdapter::set_exiting_value (double value)
{
	temp_params.exiting_value = value;
}

void AnnealingSimulationAdapter::run ()
{
	result = annealing_optimize<double>(
			energy_function,
			temp_params,
			[](auto argument_number){  },
			mutator,

			);
}

bool AnnealingSimulationAdapter::success ()
{
	return false;
}


std::vector<std::pair<size_t, double>> AnnealingSimulationAdapter::get_target_function_dynamic ()
{
	std::vector<std::pair<size_t, double>> enumerated;
	enumerated.reserve(result.current_energy_dynamic.size());
	size_t iteration_index = 0;
	for (auto& v : result.current_energy_dynamic) {
		enumerated.emplace_back(iteration_index, v );
		iteration_index++;
	}

	return enumerated;
}
