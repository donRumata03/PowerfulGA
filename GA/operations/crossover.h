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


	Population perform_dummy_crossover_matting(Parents& parents); /// DEPRECATED

	// Fresh, actual function for crossover of two genomes:
	Genome mat_parents(const std::pair<Genome*, Genome*>& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // For test

	// Simply run matting for multiple parents
	Population launch_crossover(Parents& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // Not so efficient, too much copying!

	Population apply_crossover(LightParents& parents,
	                           const std::function<Genome (const std::pair<Genome*, Genome*>& parents, const normalizer& normaaaaa, const crossover_mode mode)>& parent_matter, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); /// Used
}
