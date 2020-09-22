//
// Created by Vova on 18.09.2020.
//

#pragma once

#include <ostream>
#include "../ga_base.h"

namespace GA
{
	std::pair<double, genome> find_best_genome(population& pop, const std::vector<double>& fits);
	std::pair<double, genome> find_best_genome(light_population& pop, const std::vector<double>& fits);


	struct genome_quantities
	{
		double parent_fit_pow;
		size_t parent_number;
		size_t child_number;

		double usual_elite_fit_pow;
		size_t usual_elite_number;

		double hyper_elite_fit_pow;
		size_t hyper_elite_number;

		size_t best_genome_number;

		friend std::ostream &operator<< (std::ostream &os, const genome_quantities &quantities);
	};

	struct genome_quantity_orientation
	{
		double hazing_percent;
		double algorithm_progress_percent;

		double parent_fit_pow;
		double usual_elite_fit_pow;
		double hyper_elite_fit_pow;

		bool use_best_genome;
	};

	/**
	 * Calculates actual values from the oriented ones:
	 * @return
	 */
	genome_quantities calculate_genome_quantities(size_t space_left, const genome_quantity_orientation& orient_params);

	void renew_random();
}
