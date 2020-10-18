#pragma once
#include <pythonic.h>

#include "ga_base.h"

#include "operations/operations.h"
#include "base_GA_params.h"

constexpr bool DEBUG_GA = false;

namespace GA {

	struct single_run_GA_params : epoch_num_GA_params, _base_GA_params {};


	enum class logging_type
	{
		new_epoch,
		after_mutation,
		after_constraining,
		best_genome
	};

	inline void default_GA_informer(const double percent, const double best_fitness, const std::vector<double>& best_genome) {
		static_assert(is_printable_by_me<const std::vector<double>>::value);
		std::cout << "GA Percent: " << percent << std::endl;
		// std::cout << " ; Best fitness: " << best_fitness << " ; Best genome: " << best_genome << std::endl;
	}

	std::pair<double, genome> ga_optimize (const std::function<double (std::vector<double> &)> &fitness_function,
	                                       const std::vector<std::pair<double, double>> &point_ranges, single_run_GA_params params,
	                                       const std::function<void (double, double, const genome &)> &informer = default_GA_informer,
	                                       std::vector<double> *to_store_fitness = nullptr,
	                                       const std::function<void (const population &, size_t, logging_type)> *logger = nullptr);

	std::pair<double, genome> log_ga_optimize(
			const std::function< double(std::vector<double>&) >& target_function,
			const std::vector<std::pair<double, double>>& point_ranges, single_run_GA_params params,
			bool generate_fitness_from_loss = false, std::vector<bool> to_logariphmate = {},
			const std::function< void(double, double, const genome&) >& informer = nullptr,
			std::vector<double>* to_store_fitness = nullptr
	);

}

