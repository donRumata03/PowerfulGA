//
// Created by Vova on 18.09.2020.
//

#include "util.h"

namespace GA
{
	std::pair<double, genome> find_best_genome(population& pop, const std::vector<double>& fits)
	{
		double best_fitness = fits[0];
		genome* g = pop.data();
		for (size_t index = 0; index < pop.size(); index++) if (best_fitness < fits[index])
			{
				best_fitness = fits[index];
				g = &pop[index];
			}

		return { best_fitness, *g };
	}

	std::pair<double, genome> find_best_genome(light_population& pop, const std::vector<double>& fits)
	{
		double best_fitness = fits[0];
		genome* g = pop[0];
		for (size_t index = 0; index < pop.size(); index++) if (best_fitness < fits[index])
			{
				best_fitness = fits[index];
				g = pop[index];
			}

		return { best_fitness, *g };
	}

	void renew_random()
	{
		srand(unsigned(time(nullptr)));
		for (int i = 0; i < 100; i++) rand();
	}



	genome_quantities calculate_genome_quantities (size_t population_size, const genome_quantity_orientation &orient_params)
	{
		// TODO!!!
		return genome_quantities{

		};
	}

}
