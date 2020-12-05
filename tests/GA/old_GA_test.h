#pragma once

#include "GA/old_GA.h"
// #include "GA/binary_GA.h"
#include <iostream>

#include "tests/local_testing.h"
#include "tests/test_functions.h"

#include "tests/GA/ga_util_tests.h"

#include "quantity_tests.h"

using namespace GA;
// using namespace binary_GA;




void old_ga_test()
{
	renew_random();

	// pms ranges = { { -512, 512 }, { -512, 512 } };
	std::vector<std::pair<double, double>> ranges = { { -100, 100 }, { -100, 100 } };

	single_run_GA_params params;

	size_t total_computations = 500'000;
	params.population_size = 20'000;
	params.epoch_num = total_computations / params.population_size;



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

	params.crossover_mode = crossover_mode::low_variance_genetic;

	// std::cout << std::thread::hardware_concurrency() << " threads" << std::endl;

	// std::function<void(double, double, const vector<double>&)> callback = [];
	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best error now: " << value_by_shaffer_fit(best_fitness) << "; best genome: " << best_genome << std::endl;
	};

	auto [opt, values] = ga_optimize(shaffer_fit, ranges, params, callback, nullptr, nullptr);

	std::cout << "\n\n\nBest function value: " << value_by_shaffer_fit(opt) << " at parameters: " << values << std::endl;
}



/*
void simple_ga_test()
{
	renew_random();

	// pms ranges = { { -512, 512 }, { -512, 512 } };
	std::vector<std::pair<double, double>> ranges = { { -3, 3 } };

	GA_params params;

	size_t total_computations = 100;
	params.population_size = 20;
	params.epoch_num = total_computations / params.population_size;

	params.hazing_percent = 0.6;

	params.target_gene_mutation_number = 0.1;
	params.mutation_percent_sigma = 0.01;
	params.cut_mutations = false;
	// params.best_genome_percent = 0.05;

	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.1;

	// Multithreading parameters:
	params.allow_multithreading = false;
	params.threads = 10;

	params.exiting_fitness_value = 1e+10;

	params.crossover_mode = crossover_mode::low_variance_genetic;

	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best function value now: " << 3 + 1 / best_fitness << "; best genome: " << best_genome << std::endl;
	};

	auto [opt, values] = ga_optimize(
			[] (const std::vector<double> &vals) { return 1 / abs((vals[0] * vals[0] - 4 * vals[0] + 7) - 3); }, ranges,
			params, callback, nullptr, nullptr);

	std::cout << "\n\n\nBest function value: " << (3 + 1 / opt) << " at parameters: " << values << std::endl;
}
*/
