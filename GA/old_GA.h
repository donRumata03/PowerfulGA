#pragma once
#include <pythonic.h>

#include "ga_base.h"

#include "operations/operations.h"
#include "base_GA_params.h"

constexpr bool DEBUG_GA = false;

namespace GA {

	struct single_run_GA_params : epoch_num_GA_params, _base_GA_params {};



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

