//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

#include "crossover.h"
#include "util.h"

namespace GA
{
	/**
	 * For internal usage:
	 */
	/*
	struct generation_updating_parameters
	{
		double parent_fit_pow;

		double hyper_elite_fit_pow;
		size_t hyper_elite_number;

		double usual_elite_fit_pow;
		size_t usual_elite_number;

		size_t best_genome_number;
		genome best_genome;

		crossover_mode mode = crossover_mode::low_variance_genetic;
	};
	*/

	/// Resultant function that computes the new population:
	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa, const genome& best_genome,
			const genome_quantities& quantities, crossover_mode mode);

	// Function that divides all the population into parent pairs:
	light_parents_t distribute_pairs(light_population& pop, size_t pair_amount, bool allow_gay_marriage = false); // Actually ", bool allow_SELF_marriage = false"


	// Uses Russian roulette to select some pairs of parents:
	template<class Population_t>
	Population_t select_matting_pool(const Population_t& genomes, const std::vector<double>& fitnesses, size_t amount, double fit_dependence);

	// Work with russian roulette:
	size_t get_matting_index(std::vector<double>& russian_roulette, double value);
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
