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
		std::function<double(const Genome&)> fitness_function;
		continuous_GA_params params;

		std::vector<std::pair<double, double>> point_ranges;

		// FeedBack:
		std::vector<double> fitness_history;

		std::function<void (double, double, const Genome &)> informer = default_GA_informer;
		std::function<void (const Population &, size_t, logging_type)>* logger = nullptr; // Isn't responsible for cleaning «"your" logger» up


	public:
		/// Setting up the GA:

		GA_optimizer(std::function<double(const Genome&)> _fitness_function,
			         std::vector<std::pair<double, double>> _point_ranges,
			         continuous_GA_params _params);

		/// Copies the informer!
		void set_informer(std::function<void (double, double, const Genome &)> _informer) {
			informer = std::move(_informer);
		}

		/// Doesn`t copy the logger!
		void plug_logger(std::function<void (const Population &, size_t, logging_type)>& _logger) {
			logger = &_logger;
		}

		/**
		 *
		 *
		 * The actual GA
		 *
		 *
		 */
		struct CompletionPercent { double fraction; };

		/**
		 * For all the running functions:
		 * @return: true if current fitness is already big enough, otherwise: false
		 */
		bool run_one_iteration(const genome_quantities& quantities);
		bool run_one_iteration(CompletionPercent completion_percent);
		bool run_one_iteration(size_t max_target_epoch_num) {
			return run_one_iteration(CompletionPercent{ double(iterations_performed) / max_target_epoch_num});
		}

		bool run_many_iterations(size_t iterations, const genome_quantities& quantities);
		bool run_many_iterations(size_t iterations, size_t max_target_epoch_num);



		/// Getting run status information:
		[[nodiscard]] const std::vector<double>& get_fitness_history () const
		{
			return fitness_history;
		}

		~GA_optimizer() = default;
//		{
//			thread_pool.join();
//		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
		/// Methods:
		void _compute_fitness();


	private:

		/// Temporary run-time data:
		// Initial Configuration:
		std::vector<double> mutation_sigmas;
		bool is_multithreaded = false;

		size_t iterations_performed = 0;

		Population population;
		Genome best_genome;
		std::vector<double> fitnesses;
		std::optional<double> current_fitness = std::nullopt;

		/// Threading:
		// std::vector<std::thread> threads;
		static_thread_pool thread_pool;
		std::vector<std::pair<size_t, size_t>> thread_task_distribution;
		std::vector<double> thread_results;

		/// Utils:
		normalizer norma{10000};
	};

}

