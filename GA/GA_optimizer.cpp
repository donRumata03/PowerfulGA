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

bool GA::GA_optimizer::run_one_iteration (const genome_quantities& quantities)
{
	/// New-epoch
	if (logger) (*logger)(population, iterations_performed, logging_type::new_epoch);

	/// Mutate:
	for (auto& g : population)
		params.custom_operations.mutation(g, mutation_sigmas, params.mutation_params.target_gene_mutation_number, norma);
	if (logger) (*logger)(population, iterations_performed, logging_type::after_mutation);

	/// Cut mutations:
	if (params.mutation_params.cut_mutations) params.custom_operations.genome_constraint(population, point_ranges); // cut_mutations(p, point_ranges);
	if (logger) (*logger)(population, iterations_performed, logging_type::after_constraining);


	/// Compute fitness:
	_compute_fitness();

	/// Find best genome:
	auto best_stuff = find_best_genome(population, fitnesses);
	best_genome = std::move(best_stuff.second);
	current_fitness = best_stuff.first;
	if (!best_genome.empty() && logger) (*logger)({ best_genome }, iterations_performed, logging_type::best_genome);

	iterations_performed++;

	///
	fitness_history.push_back(*current_fitness);
	if (params.exiting_fitness_value && *current_fitness > *params.exiting_fitness_value) {
		return true;
	}

	make_new_generation(population, fitnesses, norma, best_genome, quantities, params.crossover_mode, params.custom_operations);
	return false;
}

/// Overloadings:

bool GA::GA_optimizer::run_many_iterations (size_t iterations, const GA::genome_quantities &quantities)
{
	bool is_ready = false;
	for (size_t i = 0; i < iterations; ++i) {
		is_ready = run_one_iteration(quantities);
		if (is_ready) break;
	}
	return is_ready;
}


bool GA::GA_optimizer::run_many_iterations (size_t iterations, size_t max_target_epoch_num)
{
	if (iterations_performed + iterations > max_target_epoch_num) throw std::overflow_error("Too big iteration number");
	bool is_ready = false;
	for (size_t i = 0; i < iterations; ++i) {
		is_ready = run_one_iteration(max_target_epoch_num);
		if (is_ready) break;
	}
	return is_ready;
}



/// Helpers:

bool GA::GA_optimizer::run_one_iteration (GA::GA_optimizer::CompletionPercent completion_percent)
{
	genome_quantities quantities = calculate_genome_quantities(params.population_size, {
			params.hazing_params.hazing_percent,
			completion_percent.fraction,

			params.hazing_params.parent_fit_pow,
			params.hazing_params.elite_fit_pow,
			params.hazing_params.hyper_elite_fit_pow,
			!best_genome.empty()
	});

	return run_one_iteration(quantities);
}

void GA::GA_optimizer::_compute_fitness ()
{
	if (is_multithreaded) {
		thread_pool.compute();
	}
	else {
		for (size_t genome_index = 0; genome_index < params.population_size; ++genome_index) {
			fitnesses[genome_index] = fitness_function(population[genome_index]);
		}
	}
}
