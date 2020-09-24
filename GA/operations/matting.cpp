//
// Created by Vova on 17.09.2020.
//

#include "matting.h"


namespace GA
{

	/// Resultant function:
	void make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa, const genome& best_genome,
										const genome_quantities& quantities, crossover_mode mode, const GA_operation_set& custom_operations)
	{
		/// New population is:
		///
		/// 	best_genome (given number of times)
		///			+
		/// 	hyper_elite
		///			+
		/// 	usual_elite
		///			+
		///		 children
		///

		light_population souls;

		souls.reserve(pop.size());
		for (genome& val : pop) souls.push_back(&val);

		light_population parents = select_matting_pool(souls, fitnesses, pop.size(), quantities.parent_fit_pow); /// Not the ideal ones, will come through matting

		light_population light_usual_elite = select_matting_pool(souls, fitnesses, quantities.usual_elite_number, quantities.usual_elite_fit_pow); /// Will be directly added
		light_population light_hyper_elite = select_matting_pool(souls, fitnesses, quantities.hyper_elite_number, quantities.hyper_elite_fit_pow); /// Will be directly added

		auto usual_elite = materialize_population(light_usual_elite);
		auto hyper_elite = materialize_population(light_hyper_elite);

		size_t child_number = pop.size() - quantities.usual_elite_number - quantities.hyper_elite_number - quantities.best_genome_number;
		assert(child_number == quantities.child_number);

		light_parents_t formed_pairs = distribute_pairs(parents, quantities.child_number);


		/// std::vector<genome> pop HAS NOW USELESS DATA!!! => Fill it with what we want
		/// Its size is exactly what we need!!!

		population children = apply_crossover(formed_pairs, custom_operations.parents_matting, normaaaaa, mode);

		pop.clear(); // <- Doesn`t decrease the capacity

		for (auto& child : children) pop.emplace_back(std::move(child));            // Copy children
		for (auto& elite_person : usual_elite) pop.emplace_back(std::move(elite_person));     // Copy usual elite
		for (auto& elite_person : hyper_elite) pop.emplace_back(std::move(elite_person));     // Copy hyper elite

		for (size_t index = 0; index < quantities.best_genome_number; index++) pop.emplace_back(best_genome); // Add best genomes

		assert(pop.size() == quantities.population_size);
	}



	size_t get_matting_index(std::vector<double>& russian_roulette, const double value)
	{
		size_t l = -1, r = russian_roulette.size();

		while (l != r - 1) {
			size_t mid = (r + l) / 2;
			if (russian_roulette[mid] > value) {
				r = mid;
			}
			else {
				l = mid;
			}
		}

		return r;
	}


	light_parents_t distribute_pairs(light_population& pop, const size_t pair_amount, const bool allow_gay_marriage)
	{
		light_parents_t result;
		result.reserve(pair_amount);

		size_t pop_size = pop.size();

		for(size_t p_index = 0; p_index < pair_amount; p_index++)
		{
			size_t father_index = randint(0, pop_size);
			size_t mother_index = randint(0, pop_size);
			if (!allow_gay_marriage)
				while (mother_index == father_index) mother_index = randint(0, pop_size);

			result.emplace_back(pop[mother_index], pop[father_index]);
		}
		return result;
	}


/*

	light_population select_matting_pool(const light_population& genomes, const std::vector<double>& fitnesses, const size_t amount, const double fit_dependence)
	{
		size_t size = genomes.size();
		std::vector<double> russian_roulette(size);

		double fit_sum = 0;

		for (double val : fitnesses) fit_sum += pow(val, fit_dependence);


		bool is_first = true;
		size_t index = 0;
		for (auto& fit : fitnesses) {
			double this_prob = pow(fit, fit_dependence) / fit_sum;
			russian_roulette[index] = (is_first ? this_prob : this_prob + russian_roulette[index - 1]);
			is_first = false;
			index++;
		}


		light_population res(amount);
		auto randoms = uniform_distribute(0, 1, amount);
		for (size_t i = 0; i < amount; i++) {
			auto this_value = randoms[i];
			size_t this_index = get_matting_index(russian_roulette, this_value);
			res[i] = genomes[this_index];
		}

		return res;
	}
*/


}

