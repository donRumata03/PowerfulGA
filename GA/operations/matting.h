//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

#include "crossover.h"


namespace GA
{
	enum class matting_mode
	{
		uniform,

		high_variance_normal,
		middle_variance_normal,
		low_variance_normal,

		random_variance_normal,

		high_variance_genetic,
		low_variance_genetic
	};

	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
	                               double hyper_elite_fit_pow, double usual_elite_fit_pow, double parent_fit_pow,
	                               size_t usual_elite_number, size_t hyper_elite_number, size_t best_genome_number, const genome& best_genome,
	                               matting_mode mode = matting_mode::low_variance_genetic);

	population select_matting_pool(const population& genomes, const std::vector<double>& fitnesses, size_t amount, double fit_dependence);


}
