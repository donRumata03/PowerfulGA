//
// Created by Vova on 18.10.2020.
//

#pragma once

#include <utility>

#include "ga_base.h"

#include "operations/operations.h"
#include "base_GA_params.h"


namespace GA
{

	struct continuous_GA_params : _base_GA_params
	{

	};

	class GA_optimizer
	{
	private:
		// GA params:
		std::function<double(const genome&)> fitness_function;
		continuous_GA_params params;

		std::vector<std::pair<double, double>> point_ranges;

		// FeedBack:
		std::vector<double> fitness_history;

		std::function<void (double, double, const genome &)> informer = default_GA_informer;
		std::function<void (const population &, size_t, logging_type)>* logger = nullptr; // Isn't responsible for cleaning "your" logger up


	public:
		/// Setting up the GA:

		GA_optimizer(std::function<double(const genome&)> _fitness_function,
			         std::vector<std::pair<double, double>> _point_ranges,
			         continuous_GA_params _params
//			         const std::function<void (double, double, const genome &)> &_informer = default_GA_informer,
//			         std::vector<double>* _to_store_fitness = nullptr,
//			         const std::function<void (const population &, size_t, logging_type)>* logger = nullptr
			         )
						: fitness_function(std::move(_fitness_function)),
						point_ranges(std::move(_point_ranges)),
						params(std::move(_params))

		{

		}

		/// Copies the informer!
		void set_informer(std::function<void (double, double, const genome &)> _informer) {
			informer = std::move(_informer);
		}

		/// Doesn`t copy the logger!
		void plug_logger(std::function<void (const population &, size_t, logging_type)>& _logger) {
			logger = &_logger;
		}

		/**
		 *
		 *
		 * The actual GA
		 *
		 *
		 */

		void run_one_iteration();
		void run_many_iterations(size_t iterations) { for (size_t i = 0; i < iterations; ++i) run_one_iteration(); }

		// Getting run information:
		[[nodiscard]] const std::vector<double>& get_fitness_history () const
		{
			return fitness_history;
		}
	};

}

