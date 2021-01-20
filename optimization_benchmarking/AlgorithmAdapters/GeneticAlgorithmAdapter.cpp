//
// Created by Vova on 08.12.2020.
//

#include "GeneticAlgorithmAdapter.h"
#include <GA/operations/util.h>


GeneticAlgorithmAdapter::GeneticAlgorithmAdapter (std::function<double (const GA::Genome&)> fitness_function,
                                                  std::vector<std::pair<double, double>> point_ranges,
                                                  GA::continuous_GA_params params,
                                                  double _epoch_pow) : epoch_pow(_epoch_pow)
{
	// optimizer = std::make_shared<GA::GA_optimizer>(fitness_function, point_ranges, params);

	temp_fitness_function = std::move(fitness_function);
	temp_point_ranges = std::move(point_ranges);
	temp_params = std::move(params);
}

void GeneticAlgorithmAdapter::set_computations (size_t computations)
{
	auto distribution = GA::distribute_computations_defaultly(computations, epoch_pow);

	temp_params.population_size = distribution.population_size;
	computed_epoch_number = distribution.epoch_number;
}

void GeneticAlgorithmAdapter::set_exiting_value (double value)
{
	temp_params.exiting_fitness_value = value;
}

void GeneticAlgorithmAdapter::run ()
{
	optimizer = std::make_shared<GA::GA_optimizer>(temp_fitness_function, temp_point_ranges, temp_params);

	m_success = optimizer->run_all_iterations(computed_epoch_number);
}

bool GeneticAlgorithmAdapter::success ()
{
	return m_success;
}

std::vector<std::pair<size_t, double>> GeneticAlgorithmAdapter::get_target_function_dynamic ()
{
	auto history_by_iterations = optimizer->get_fitness_history();

	std::vector<std::pair<size_t, double>> res(history_by_iterations.size());
	size_t hist_index = 0;
	for(auto & h : history_by_iterations) {
		res[hist_index] = { hist_index * temp_params.population_size, h };
	}
	return res;
}
