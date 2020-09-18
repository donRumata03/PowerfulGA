//
// Created by Vova on 17.09.2020.
//

#include "matting.h"


namespace GA
{
	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
	                               const double hyper_elite_fit_pow, const double usual_elite_fit_pow, const double parent_fit_pow,
	                               const size_t usual_elite_number, const size_t hyper_elite_number, const size_t best_genome_number, const genome& best_genome,
	                               const matting_mode mode)
	{
		light_population souls;

		souls.reserve(pop.size());
		for (genome& val : pop) souls.push_back(&val);

		light_population parents = select_matting_pool(souls, fitnesses, pop.size(), parent_fit_pow);
		light_population usual_elite = select_matting_pool(souls, fitnesses, usual_elite_number, usual_elite_fit_pow);
		light_population hyper_elite = select_matting_pool(souls, fitnesses, hyper_elite_number, hyper_elite_fit_pow);


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


}

