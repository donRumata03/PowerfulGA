//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

namespace GA
{
	void mutate(genome& target_genome, const std::vector<double>& sigmas, double target_gene_number, const normalizer& normaaa);
	void cut_mutations(population& population, const std::vector<std::pair<double, double>>& constraints);
}
