#pragma once
#include <pythonic.h>

#include "ga_base.h"

#include "operations/mutation.h"
#include "operations/population_generation.h"
#include "operations/util.h"
#include "operations/crossover.h"
#include "operations/matting.h"

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

	/// DELETED: double best_genome_percent = 0.05;

	double hyper_elite_fit_pow = 5;
	double elite_fit_pow = 2;
	double parent_fit_pow = 0.3;

	GA::crossover_mode crossover_mode = GA::crossover_mode::low_variance_genetic;

	double hazing_percent = 0.3;
	std::optional<double> exiting_fitness_value = {};

	GA::GA_operation_set custom_operations = {};

	void set_default_epoch_num(size_t total_computations) {
		/// TODO: find the optimal fraction by testing:
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
};

namespace GA {
	inline void default_GA_informer(const double percent, const double best_fitness, const std::vector<double>& best_genome) {
		static_assert(is_printable_by_me<const std::vector<double>>::value);
		std::cout << "GA Percent: " << percent;
		// std::cout << " ; Best fitness: " << best_fitness << " ; Best genome: " << best_genome << std::endl;
	}

	std::pair<double, genome> ga_optimize(
			const std::function< double(std::vector<double>&) >& fitness_function,
			const std::vector<std::pair<double, double>>& point_ranges, GA_params params,
			const std::function< void(double, double, const genome&) >& informer = default_GA_informer,
		                                  std::vector<double>* to_store_fitness = nullptr);

	std::pair<double, genome> log_ga_optimize(
			const std::function< double(std::vector<double>&) >& target_function,
			const std::vector<std::pair<double, double>>& point_ranges, GA_params params,
			bool generate_fitness_from_loss = false, std::vector<bool> to_logariphmate = {},
			const std::function< void(double, double, const genome&) >& informer = nullptr,
			std::vector<double>* to_store_fitness = nullptr
	);

}