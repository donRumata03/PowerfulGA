//
// Created by Vova on 17.09.2020.
//

#include "mutation.h"


namespace GA
{
	void mutate(Genome& target_genome, const std::vector<double>& sigmas, const double target_gene_number, const normalizer& normaaa)
	{
		auto real_gene_number = static_cast<size_t>(cut(std::round(normaaa.generate(target_gene_number, target_gene_number / 2)), 0, target_genome.size()));
		std::vector<size_t> mutate_indexes = uniform_int_distribute(0, target_genome.size() - 1, real_gene_number);
		// vector<double> mutate_values = normal_distribute(0, sigma, real_gene_number);
		// cout << mutate_indexes << " " << mutate_values << endl;
		for (size_t index = 0; index < mutate_indexes.size(); index++) {
			double this_mutate_value = normaaa.generate(0, sigmas[index]);
			target_genome[mutate_indexes[index]] += this_mutate_value;
		}
	}

	void cut_mutations (Population &population, const std::vector<std::pair<double, double>> &constraints)
	{
		for (auto& genome : population) {
			for (size_t gene_index = 0; gene_index < genome.size(); gene_index++) {
				genome[gene_index] = std::clamp(genome[gene_index], constraints[gene_index].first, constraints[gene_index].second);
			}
		}
	}


}

