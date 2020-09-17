#pragma once
#include <pythonic.h>
#include <functional>
#include <optional>

#include "ga_util.h"

constexpr bool DEBUG_GA = false;

struct GA_params
{
	size_t population_size = 0;
	size_t epoch_num = 0;


	double mutation_percent_sigma = 0;
	double target_gene_mutation_number = 0;
	bool cut_mutations = false;

	bool allow_multithreading = false;
	size_t threads = std::thread::hardware_concurrency() - 2;

	double best_genome_percent = 0.05; // TODO!

	double hyper_elite_pow = 5; // TODO!
	double elite_fit_pow = 2;
	double parent_fit_pow = 0.3;


	GA::matting_mode mode_of_matting = GA::matting_mode::low_variance_genetic;

	double hazing_percent = 0.3;
	std::optional<double> exiting_fitness_value = {};
};

namespace GA {
	std::pair<double, genome> ga_optimize(
			const std::function< double(std::vector<double>&) >& fitness_function,
			const std::vector<std::pair<double, double>>& point_ranges, GA_params params,
			const std::function< void(double, double, const genome&) >& informer = nullptr,
		                                  std::vector<double>* to_store_fitness = nullptr);

	std::pair<double, genome> log_ga_optimize(
			const std::function< double(std::vector<double>&) >& target_function,
			const std::vector<std::pair<double, double>>& point_ranges, GA_params params,
			bool generate_fitness_from_loss = false, std::vector<bool> to_logariphmate = {},
			const std::function< void(double, double, const genome&) >& informer = nullptr,
			std::vector<double>* to_store_fitness = nullptr
	);

}