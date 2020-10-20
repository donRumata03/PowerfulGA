//
// Created by Vova on 21.10.2020.
//

#pragma once

#include "GA/GA_optimizer.h"
#include "test_functions.h"

inline void test_GA() {
	std::vector<std::pair<double, double>> ranges = { { -100, 100 }, { -100, 100 } };

	GA::continuous_GA_params params;

	size_t total_computations = 500'000;
	params.population_size = 20'000;
	size_t epochs = total_computations / params.population_size;
	// params.epoch_num = total_computations / params.population_size;



	params.hazing_params.hazing_percent = 0.7;

	params.mutation_params.target_gene_mutation_number = 0.1;
	params.mutation_params.mutation_percent_sigma = 0.01;
	params.mutation_params.cut_mutations = false;
	// params.best_genome_percent = 0.05;

	params.hazing_params.elite_fit_pow = 2;
	params.hazing_params.parent_fit_pow = 0.3;

	// Multithreading parameters:
	params.threading_params.allow_multithreading = true;
	params.threading_params.threads = 10;

	params.crossover_mode = GA::crossover_mode::low_variance_genetic;

	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best error now: " << value_by_shaffer_fit(best_fitness) << "; best genome: " << best_genome << std::endl;
	};

	// auto [opt, values] = ga_optimize(shaffer_fit, ranges, params, callback, nullptr, nullptr);

	GA::GA_optimizer optimizer(shaffer_fit, ranges, params);
	optimizer.set_informer(callback);

	optimizer.run_many_iterations(epochs, epochs);

	double fitness = *optimizer.get_current_fitness();
	auto fitness_history = optimizer.get_fitness_history();
	auto best_genome = optimizer.get_best_genome();

	std::cout << "\n\n\nBest function value: " << value_by_shaffer_fit(fitness) << " at parameters: " << best_genome << std::endl;
}
