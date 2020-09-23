#include "GA.h"



namespace GA {

	// TODO: add possibility to run 2 GA's at once!!!
	population p;
	std::vector<double> fitnesses;
	volatile bool thread_count_time = false;
	std::vector<bool> threads_ready;
	volatile bool threads_work_collected = false;
	
	inline bool check_threads() { return std::all_of(threads_ready.begin(), threads_ready.end(), [](const bool stuff) { return stuff; }); }
	
	bool thread_task_ready = false;
	
	void count_fitness_function_thread(const size_t thread_index, const std::function< double(std::vector<double>&) >& fitness_function, const std::pair<size_t, size_t> range)
	{
		// cout << "Thread " << thread_index << " started working! " << range << "\n";
		while(!thread_task_ready)
		{
			while(!thread_count_time && !thread_task_ready) { std::this_thread::yield(); }

			if (thread_task_ready)
			{
				break;
			}
			
			for(size_t task_index = range.first; task_index < range.second; task_index++)
			{
				fitnesses[task_index] = fitness_function(p[task_index]);
			}

			threads_ready[thread_index] = true;
			
			while (!threads_work_collected) { std::this_thread::yield(); }
		}
	}

	std::pair<double, genome> ga_optimize(const std::function< double(std::vector<double>&) >& fitness_function,
		const std::vector<std::pair<double, double>>& point_ranges, const GA_params params,
		const std::function< void(double, double, const genome&) >& informer, std::vector<double>* to_store_fitness)
	{
		// Unpacking parameters:
	/*
	auto population_size = params.population_size;
	auto epoch_num = params.epoch_num;
	auto mutation_percent_sigma = params.mutation_percent_sigma;
	bool allow_multithreading = params.allow_multithreading;
	size_t thread_number = params.threads;
	*/

/*
		bool best_genome_guarantee = params.best_genome_percent != 0;
		auto best_genome_number = size_t(std::round(params.best_genome_percent * population_size));
*/
/*

		size_t non_best_number = population_size - best_genome_number;
		
		double hazing_percent = params.hazing_percent;
		auto all_elite_number = size_t(cut(std::round(hazing_percent * non_best_number), 0, population_size));

		auto hyper_elite_number = size_t(cut(std::round(hazing_percent * all_elite_number), 0, population_size));
		size_t usual_elite_number = cut(all_elite_number - hyper_elite_number, 0, population_size);

		size_t child_number = non_best_number - all_elite_number;

		auto matting_mode = params.crossover_mode;
*/
/*

		std::cout << "People distribution: Non best: " << non_best_number << " Childs: " << child_number <<
		" All elite: " << all_elite_number << " Usual elite: " << usual_elite_number << " Hyper elite: " << hyper_elite_number
		<< " Best genome: " << best_genome_number << std::endl;
*/

/*
		double temp_pop = population_size * parents_percent;
		size_t pop_parents_number = (size_t(temp_pop) % 2) ? size_t(temp_pop) + 1 : size_t(temp_pop);
		if constexpr (DEBUG_GA) cout << "Replications per iteration: " << pop_parents_number << endl << endl; 
		*/

		/*
		 * Parameter Interpretation and other preparations:
		 */

		std::vector<double> mutation_sigmas;
		mutation_sigmas.reserve(point_ranges.size());
		for (auto& range : point_ranges) mutation_sigmas.push_back((range.first - range.second) * params.mutation_percent_sigma);
		// if constexpr (DEBUG_GA) std::cout << "Mutation sigmas: " << mutation_sigmas << std::endl << std::endl;

		
		// Generating initial population
		p = generate_population(point_ranges, params.population_size);
		// if constexpr (DEBUG_GA) std::cout << "Initial population: " << p << std::endl << std::endl;

		// Init fitness vectors:
		fitnesses.assign(params.population_size, 0.);
		if (to_store_fitness) to_store_fitness->reserve(params.epoch_num);
		
		// Making threads
		std::vector<std::thread> threads;
		if(params.allow_multithreading)
		{
			threads_ready.assign(params.threads, false);

			auto task_ranges = distribute_task_ranges(params.population_size, params.threads);
			
			threads.reserve(params.threads);
			for (size_t thread_index = 0; thread_index < params.threads; thread_index++)
			{
				threads.emplace_back(count_fitness_function_thread, thread_index, ref(fitness_function), task_ranges[thread_index]);
			}
		}

		// Init fast normalizer:
		normalizer normaaaaa(10000);
		

		/// Beginning of the main loop:
		double current_fitness = 0;
		genome best_genome;

		for (size_t epoch = 0; epoch < params.epoch_num; epoch++) {
			/// Mutation
			for (auto& g : p) mutate(g, mutation_sigmas, params.target_gene_mutation_number, normaaaaa);
			if (params.cut_mutations) cut_mutations(p, point_ranges);

			// if constexpr (DEBUG_GA) std::cout << "After mutation: " << p << std::endl << std::endl;

			// Calculating fitnesses : MULTITHREADING!
			// vector<double> fitnesses(population_size);
			if (params.allow_multithreading) {
				threads_work_collected = false;				
				// Say other threads, that it`s time to count:
				thread_count_time = true;

				/*
				 *  Threads are counting now
				 */
				
				// Wait while they`re not ready:
				while(!check_threads()) {}

				thread_count_time = false;

				threads_work_collected = true;
				// fitnesses = thread_results; 
			}
			else { // If multithreading in`t allowed:
				for (size_t index = 0; index < params.population_size; index++) {
					fitnesses[index] = fitness_function(p[index]);
				}
			}

			
			// Find the best genome and its fitness:
			auto [_best_fitness, _best_genome] = find_best_genome(p, fitnesses);
			best_genome = _best_genome;
			current_fitness = _best_fitness;

			if (params.exiting_fitness_value && current_fitness > *params.exiting_fitness_value) break;
			if (to_store_fitness) to_store_fitness->push_back(current_fitness);

			// Calculating population quantities:
			auto population_quantities = calculate_genome_quantities(params.population_size,
					{
							params.hazing_percent,
						double(epoch) / params.epoch_num,

						params.parent_fit_pow,
						params.elite_fit_pow,
						params.hyper_elite_fit_pow,
						epoch != 0
					});

			// Making new population, the most interesting part:
			p = make_new_generation(p, fitnesses, normaaaaa, best_genome, population_quantities, params.crossover_mode);

			
			// if constexpr (DEBUG_GA) std::cout << "Fitness functions: " << fitnesses << std::endl;

			// Output information:
			informer(100 * double(epoch) / params.epoch_num, current_fitness, best_genome);
		}
		/// ^^^ End Of Main Loop ^^^

		if(params.allow_multithreading) {
			// Stop other threads:
			thread_task_ready = true;
			for (auto &this_thread : threads) this_thread.join();
		}

		// Here`s the actual result:
		return { current_fitness, best_genome };
	}
	
	// Log Wrapper
	std::pair<double, genome> log_ga_optimize(const std::function<double(std::vector<double>&)>& target_function, std::vector<std::pair<double, double>>& point_ranges,
		const GA_params params, const bool generate_fitness_from_loss, std::vector<bool> to_logariphmate,
		const std::function< void(double, double, const genome&) >& informer, std::vector<double>* to_store_fitness)
	{

		if (to_logariphmate.empty()) to_logariphmate.assign(point_ranges.size(), false);
		else assert(to_logariphmate.size() == point_ranges.size());

		// Cook fake ranges:
		auto real_point_ranges = point_ranges;
		for (size_t r_index = 0; r_index < point_ranges.size(); r_index++) if (to_logariphmate[r_index]) {
			real_point_ranges[r_index] = { log(point_ranges[r_index].first), log(point_ranges[r_index].second) };
		}

		auto wrapper_lambda = [&](std::vector<double>& values) -> double
		{
			std::vector<double> fake_values;
			fake_values.reserve(values.size());
			for (size_t value_index = 0; value_index < values.size(); value_index++)
			{
				fake_values.push_back(to_logariphmate[value_index] ? exp(values[value_index]) : values[value_index]);
			}
			auto function_result = target_function(fake_values);
			return generate_fitness_from_loss ? 1 / function_result : function_result;
		};

		auto informer_wrapper = [&](const double GA_percent, const double best_fitness, const std::vector<double>& best_genome)
		{
			std::vector<double> fake_genome = best_genome;
			for (size_t index = 0; index < best_genome.size(); index++) if (to_logariphmate[index]) fake_genome[index] = exp(fake_genome[index]);
			informer(GA_percent, best_fitness, fake_genome);
		};

		std::pair<double, genome> ga_result = ga_optimize(wrapper_lambda, real_point_ranges, params, informer_wrapper, to_store_fitness);
		std::pair<double, genome> real_result = { ((generate_fitness_from_loss) ? (1 / ga_result.first) : (ga_result.first)), ga_result.second };
		return real_result;
	}
}
