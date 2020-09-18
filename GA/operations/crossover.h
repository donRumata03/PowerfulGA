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


	population perform_crossover_matting(parents_t& parents); // Deprecated
	light_parents_t distribute_pairs(light_population& pop, size_t pair_amount, bool allow_gay_marriage = false);

	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // For test
	population perform_crossover(parents_t& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // Fancy
	population perform_crossover(light_parents_t& parents, const normalizer& normaaaaa, crossover_mode mode = crossover_mode::low_variance_genetic); // Fancy
}
