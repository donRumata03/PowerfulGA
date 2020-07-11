#pragma once

#include "../GA/ga_util.h"

namespace binary_GA
{

	using genome = vector<std::bitset<64>>;
	using population = vector<genome>;
	using parents_t = vector<pair<genome, genome>>;

	population generate_population(const pms& ranges, size_t amount);
	void mutate(genome& target_genome, const vector<double>& sigmas, double target_gene_number);

	vector<vector<double>> select_matting_pool(const population& genomes, const vector<double>& fitnesses, size_t amount);
	population perform_crossover_matting(parents_t& parents);
}
