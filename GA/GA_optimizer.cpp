//
// Created by Vova on 18.10.2020.
//

#include "GA_optimizer.h"

/**
 * Initialization
 */

GA::GA_optimizer::GA_optimizer (std::function<double (const Genome &)> _fitness_function,
                                std::vector<std::pair<double, double>> _point_ranges, GA::continuous_GA_params _params)
		: fitness_function(std::move(_fitness_function)),
		  point_ranges(std::move(_point_ranges)),
		  params(std::move(_params))

{
	/// Init mutation sigmas:
	if (params.mutation_params.individual_mutation_sigmas) {
		mutation_sigmas = *params.mutation_params.individual_mutation_sigmas;
	}
	else {
		mutation_sigmas.reserve(point_ranges.size());
		for (auto &range : point_ranges) {
			mutation_sigmas.push_back((range.first - range.second) * params.mutation_params.mutation_percent_sigma);
		}
	}

	/// Init population:
	population = params.custom_operations.population_generation(point_ranges, params.population_size);

	/// Init fitnesses:
	fitnesses.assign(params.population_size, 0.);

	/// Threading:
	is_multithreaded = params.threading_params.allow_multithreading;
	if (is_multithreaded) {
		thread_task_distribution = distribute_task_ranges(params.population_size, params.threading_params.threads);
		thread_results.assign(params.threading_params.threads, 0.);

		thread_pool.init(params.threading_params.threads, [&](size_t thread_index){
			for (size_t task_index = thread_task_distribution[thread_index].first; task_index < thread_task_distribution[thread_index].second; ++task_index) {
				thread_results[task_index] = fitness_function(population[task_index]);
			}
		});

	}

	///
}


/**
 * Computation
 */

void GA::GA_optimizer::run_one_iteration (const genome_quantities& quantities)
{



	iterations_performed++;
}


/// Helper:

void GA::GA_optimizer::run_one_iteration (GA::GA_optimizer::CompletionPercent completion_percent)
{
	genome_quantities quantities = calculate_genome_quantities(params.population_size, {
			params.hazing_params.hazing_percent,
			completion_percent.fraction,

			params.hazing_params.parent_fit_pow,
			params.hazing_params.elite_fit_pow,
			params.hazing_params.hyper_elite_fit_pow,
			!best_genome.empty()
	});

	run_one_iteration(quantities);
}


