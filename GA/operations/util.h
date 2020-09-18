//
// Created by Vova on 18.09.2020.
//

#pragma once

#include "../ga_base.h"

namespace GA
{
	std::pair<double, genome> find_best_genome(population& pop, const std::vector<double>& fits);
	std::pair<double, genome> find_best_genome(light_population& pop, const std::vector<double>& fits);
}
