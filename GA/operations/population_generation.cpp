//
// Created by Vova on 17.09.2020.
//

#include "population_generation.h"

namespace GA
{
	Population generate_population(const std::vector<std::pair<double, double>>& ranges, size_t amount)
	{
		size_t genome_size = ranges.size();
		Population res(amount);

		std::vector<std::vector<double>> genome_randoms(genome_size);
		for (int i = 0; i < genome_size; i++) {
			genome_randoms[i] = uniform_distribute(ranges[i].first, ranges[i].second, amount);
		}


		for (size_t i = 0; i < amount; i++) {
			res[i].reserve(genome_size);
			for (size_t j = 0; j < genome_size; j++) {
				res[i].emplace_back(genome_randoms[j][i]);
			}
		}

		return res;
	}

}

