//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

#include "crossover.h"


namespace GA
{

	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
	                               double hyper_elite_fit_pow, double usual_elite_fit_pow, double parent_fit_pow,
	                               size_t usual_elite_number, size_t hyper_elite_number, size_t best_genome_number, const genome& best_genome,
	                               crossover_mode mode = crossover_mode::low_variance_genetic);


	template<class Population_t>
	Population_t select_matting_pool(const Population_t& genomes, const std::vector<double>& fitnesses, size_t amount, double fit_dependence);
	// light_population select_matting_pool(const light_population& genomes, const std::vector<double>& fitnesses, const size_t amount, const double fit_dependence);

}

/// Template function implementations:

namespace GA
{
	template<class Population_t>
	Population_t select_matting_pool(const Population_t& genomes, const std::vector<double>& fitnesses, const size_t amount, const double fit_dependence)
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


		Population_t res(amount);
		auto randoms = uniform_distribute(0, 1, amount);
		for (int i = 0; i < amount; i++) {
			auto this_value = randoms[i];
			size_t this_index = get_matting_index(russian_roulette, this_value);
			res[i] = genomes[this_index];
		}

		return res;

	}
}
