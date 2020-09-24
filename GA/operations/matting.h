//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

#include "util.h"
#include "crossover.h"
#include "mutation.h"
#include "population_generation.h"

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

	/**
	 * Allows to customize all the useful operations with genome:
	 */
	struct GA_operation_set
	{
		/// Population Generation:
		std::function<population(const std::vector<std::pair<double, double>>& ranges, size_t amount)> population_generation = generate_population;

		/// Genome mutation:
		std::function<void(genome& target_genome, const std::vector<double>& sigmas, double target_gene_number, const normalizer& normaaa)> mutation = mutate;

		/// Genome selection isn`t customizable, because, come on, it doesn`t depend on the nature of the genomes but only on the fitness function result and on the method of selection

		/// Pair Distribution is also non-customizable for the same reasons

		/// Function For actual matting
		std::function<genome (const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, const crossover_mode mode)> parents_matting = mat_parents;
	};


	/**
	 * 	Binary GA operation Preset: works with std::vector<std::bitset<...>> instead of std::vector<double>, so need templates for GA main function!!!
	 */
	static inline GA_operation_set binary_GA_operation_preset = {
			// TODO!!! : use templates for genome type!!!
			/// .population_generation = ;
	};

	/// Resultant function that computes the new population:
	void make_new_generation(population& pop,const std::vector<double>& fitnesses, const normalizer& normaaaaa, const genome& best_genome,
			const genome_quantities& quantities, crossover_mode mode, const GA_operation_set& custom_operations);

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
