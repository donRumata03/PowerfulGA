//
// Created by Vova on 18.10.2020.
//

#pragma once

#include "operations/operations.h"

namespace GA
{

	struct epoch_num_GA_params
	{
		size_t epoch_num = 0;
	};

	struct hazing_GA_params
	{
		double hyper_elite_fit_pow = 5;
		double elite_fit_pow = 2;
		double parent_fit_pow = 0.3;

		double hazing_percent = 0.4;
	};

	struct mutation_GA_params {
		double mutation_percent_sigma = 0;
		double target_gene_mutation_number = 0;
		bool cut_mutations = true;
		std::optional<std::vector<double>> individual_mutation_sigmas; // Allows to specify the mutation sigma individually for each number in genome
	};

	struct threading_GA_params
	{
		bool allow_multithreading = false;
		size_t threads = std::thread::hardware_concurrency() - 2;
	};



	struct _base_GA_params
	{
		// size_t epoch_num = 0;
		size_t population_size = 0;

		hazing_GA_params hazing_params;
		mutation_GA_params mutation_params;
		GA::crossover_mode crossover_mode = GA::crossover_mode::low_variance_genetic;

		std::optional<double> exiting_fitness_value = {};

		threading_GA_params threading_params;
		GA::GA_operation_set custom_operations = {};
/*
		void set_default_epoch_num(size_t total_computations) {
			if (total_computations < 100) {
				double temp_population_size = std::pow(double(total_computations), 0.75);
				epoch_num = size_t(total_computations / temp_population_size);
				population_size = size_t(temp_population_size);
			}
			else {
				epoch_num = size_t(10. * std::log(double(total_computations)) / std::log(10'000));
				population_size = total_computations / epoch_num;
			}
		}
*/
	};

}
