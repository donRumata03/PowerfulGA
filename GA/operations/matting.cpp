//
// Created by Vova on 17.09.2020.
//

#include "matting.h"


namespace GA
{

	/// Resultant function:
	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
	                               const double hyper_elite_fit_pow, const double usual_elite_fit_pow, const double parent_fit_pow,
	                               const size_t usual_elite_number, const size_t hyper_elite_number, const size_t best_genome_number, const genome& best_genome,
	                               const crossover_mode mode)
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

		souls.reserve(pop.size()); //
		for (genome& val : pop) souls.push_back(&val);

		light_population parents = select_matting_pool(souls, fitnesses, pop.size(), parent_fit_pow); /// Not the ideal ones, will come through matting

		light_population usual_elite = select_matting_pool(souls, fitnesses, usual_elite_number, usual_elite_fit_pow); /// Will be directly added
		light_population hyper_elite = select_matting_pool(souls, fitnesses, hyper_elite_number, hyper_elite_fit_pow); /// Will be directly added


		size_t child_number = pop.size() - usual_elite_number - hyper_elite_number - best_genome_number;


		// cout << "Parents: " << endl;
		// for (auto parent : parents) cout << *parent << endl;

		// cout << "Elite: " << endl;
		// for (auto& p : elite) cout << *p << endl;


		light_parents_t formed_pairs = distribute_pairs(parents, child_number);

		// cout << "Best: " << 1 / find_best_genome(pop, fitnesses).first << " " << find_best_genome(pop, fitnesses).second << endl;

		// cout << endl << "Pairs: " << endl;
		// for (auto& p : formed_pairs) cout << *p.first << " " << *p.second << endl;

		population res = perform_crossover(formed_pairs, normaaaaa, mode);

		res.reserve(pop.size());

		// cout << res << endl;

		for (auto& elite_person : usual_elite) res.emplace_back(*elite_person);
		for (auto& elite_person : hyper_elite) res.emplace_back(*elite_person);

		for (size_t index = 0; index < best_genome_number; index++) res.emplace_back(best_genome);

		return res;
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

