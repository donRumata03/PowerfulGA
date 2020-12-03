//
// Created by Vova on 03.12.2020.
//

#pragma once

#include <pythonic.h>

double exp_temperature_dynamic(double completion_percent)
{
	return std::exp(-3 * completion_percent);
}

template <class T>
struct permutational_mutation
{
	double quantity_coefficient;

	explicit permutational_mutation(double _quantity_coefficient)
		: quantity_coefficient(_quantity_coefficient) {}

	std::vector<T> operator() (const std::vector<T>& genome, double quantity) const
	{
		auto res = genome;

		auto genes_to_permute = size_t(std::round(std::max(0., quantity_coefficient * quantity)));

		for (size_t p_index = 0; p_index < genes_to_permute; ++p_index) {
			size_t first_index = randint(0, genome.size());
			size_t second_index = randint(0, genome.size());

			std::swap(res[first_index], res[second_index]);
		}

		return res;
	}
};


struct modificational_mutation
{
	double typical_gene_mutation_number = 0;
	double typical_mutation_coefficient = 0;
	std::optional<std::pair<double, double>> gene_values_restrictions;

	modificational_mutation(double typical_gene_number, double typical_mutation_coefficient)
									: typical_gene_mutation_number(typical_gene_number), typical_mutation_coefficient(typical_mutation_coefficient)
	{}

	std::vector<double> operator() (const std::vector<double>& genome, double quantity) const {
		auto res = genome;

		double target_gene_mutation_number = typical_gene_mutation_number * quantity;
		double target_gene_delta = typical_mutation_coefficient * quantity;

		double float_gene_mutation_number = std::clamp(
			normal_distribute(target_gene_mutation_number, target_gene_mutation_number / 2, 1)[0],
		0., double(genome.size()));
		auto gene_mutation_number = size_t(std::round(float_gene_mutation_number));

		std::vector<double> gene_mutations = normal_distribute(0., target_gene_delta, gene_mutation_number);

		for (size_t mutation_index = 0; mutation_index < gene_mutation_number; ++mutation_index) {
			size_t gene_position = randint(0, genome.size());
			res[gene_position] += gene_mutations[mutation_index];
		}

		return res;
	}
};

// struct genome_generator {
//
//};

