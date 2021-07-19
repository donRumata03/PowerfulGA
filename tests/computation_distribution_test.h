//
// Created by Vova on 19.07.2021.
//

#pragma once

#include <GA/GA_optimizer.h>

inline double run_GA_with_iteration_distribution(double ep_pow = 0.35, bool mute = false) {
	size_t computations = 5000;

	auto distr = GA::distribute_computations_defaultly(computations, ep_pow);
	if (not mute) std::cout << distr << std::endl;

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
		if (not mute) std::cout << "GA: " << percent_plotter(iterations_performed, distr.epoch_number, 2) << " iterations performed; best error now: " << value_by_shaffer_fit(best_fitness) << "; best genome: " << best_genome << std::endl;
	};
	optimizer.set_informer(callback);

	optimizer.run_all_iterations(distr.epoch_number);

	double fitness = *optimizer.get_current_fitness();
	auto fitness_history = optimizer.get_fitness_history();
	auto best_genome = optimizer.get_best_genome();

	if (not mute) std::cout << "\n\n\nBest function value: " << value_by_shaffer_fit(fitness) << " at parameters: " << best_genome << std::endl;

	return fitness;
}

inline void explore_performance_vs_epoch_pow() {
	size_t repetitions = 100;
	double start = 0.1;
	double end = 0.75;
	double step = 0.05;

	std::vector<double> upper_dependency;
	std::vector<double> lower_dependency;
	auto ep_pows = arange(start, end, step);

	for (auto this_ep_pow : ep_pows) {
		std::vector<double> this_fits;
		for (size_t i = 0; i < repetitions; ++i) {
			this_fits.push_back(run_GA_with_iteration_distribution(this_ep_pow, true));
		}
		auto upper_mean = n_atic_mean(this_fits, 4.5);
		auto lower_mean = n_atic_mean(this_fits, 0.5);
		upper_dependency.push_back(upper_mean);
		lower_dependency.push_back(lower_mean);
		std::cout << "Epoch pow: " << this_ep_pow
			<< ". Upper mean fit: " << upper_mean << " ≈Upper mean error: " << value_by_shaffer_fit(upper_mean)
			<< "; Lower mean fit: " << lower_mean << " ≈Lower mean error: " << value_by_shaffer_fit(lower_mean)
			<< " (values: " << this_fits << ")" << std::endl << std::endl;
	}


	for (const auto& p : { std::pair { lower_dependency, "lower"s }, std::pair{ upper_dependency, "upper"s } }) {
		const auto& [this_dependency, this_name] = p;

		auto this_smoothed = smooth(zip(ep_pows, this_dependency), 0.1, 500);
		add_vectors_to_plot(ep_pows, this_dependency, plot_params { .name = this_name + " (measured points)", .type = "scatter",});
		add_pairs_to_plot(this_smoothed, { .name = this_name + " (smoothed)" });
	}



	show_plot();
}
