//
// Created by Vova on 18.09.2020.
//

#pragma once

#include "../ga_base.h"

namespace GA
{
	population perform_crossover_matting(parents_t& parents); // Deprecated
	light_parents_t distribute_pairs(light_population& pop, size_t pair_amount, bool allow_gay_marriage = false);

	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, matting_mode mode = matting_mode::low_variance_genetic); // For test
	population perform_crossover(parents_t& parents, const normalizer& normaaaaa,  matting_mode mode = matting_mode::low_variance_genetic); // Fancy
	population perform_crossover(light_parents_t& parents, const normalizer& normaaaaa,  matting_mode mode = matting_mode::low_variance_genetic); // Fancy
}
