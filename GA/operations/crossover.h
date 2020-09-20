//
// Created by Vova on 18.09.2020.
//

#pragma once

#include "../ga_base.h"

namespace GA
{
	enum class crossover_mode
	{
		uniform,

		high_variance_normal,
		middle_variance_normal,
		low_variance_normal,

		random_variance_normal,

		high_variance_genetic,
		low_variance_genetic
	};


	population perform_dummy_crossover_matting(parents_t& parents); /// DEPRECATED

	// Fresh, actual function for crossover of two genomes:
	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // For test

	// Simply run matting for multiple parents
	population launch_crossover(parents_t& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // Not so efficient, too much copying!
	population launch_crossover(light_parents_t& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); /// Used
}
