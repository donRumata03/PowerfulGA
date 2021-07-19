//
// Created by Vova on 19.07.2021.
//

#pragma once

#include <GA/GA_optimizer.h>

inline void test_iteration_distribution() {
	double ep_pow = 0.2;
	size_t computations = 5000;

	auto distr = GA::distribute_computations_defaultly(computations, ep_pow);
	std::cout << distr << std::endl;

	GA::continuous_GA_params params{};
	params.mutation_params = {
			.mutation_percent_sigma = 0.15,
			.target_gene_mutation_number = 0.4,
			.cut_mutations = true,
	};
	params.population_size = distr.population_size;
	std::vector<std::pair<double, double>> ranges = { { -100, 100 }, { -100, 100 } };

	GA::GA_optimizer optimizer(shaffer_fit, ranges, params);
	auto callback = [&](const size_t iterations_performed, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA: " << percent_plotter(iterations_performed, distr.epoch_number, 2) << " iterations performed; best error now: " << value_by_shaffer_fit(best_fitness) << "; best genome: " << best_genome << std::endl;
	};
	optimizer.set_informer(callback);

	optimizer.run_all_iterations(distr.epoch_number);

	double fitness = *optimizer.get_current_fitness();
	auto fitness_history = optimizer.get_fitness_history();
	auto best_genome = optimizer.get_best_genome();

	std::cout << "\n\n\nBest function value: " << value_by_shaffer_fit(fitness) << " at parameters: " << best_genome << std::endl;
}
