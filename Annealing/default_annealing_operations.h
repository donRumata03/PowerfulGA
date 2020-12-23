//
// Created by Vova on 03.12.2020.
//

#pragma once

#include <pythonic.h>

#include <utility>

inline double exp_temperature_dynamic(double completion_percent, double intensity = 3.5)
{
	return std::exp(-intensity * completion_percent);
}

class custom_exp_temperature_dynamic
{
public:
	explicit custom_exp_temperature_dynamic(double _intensity) : intensity(_intensity) {}

	double operator() (double completion_percent) const {
		return exp_temperature_dynamic(completion_percent, intensity);
	}
private:
	double intensity;
};

template <class T>
struct permutational_mutation
{
	double quantity_coefficient;

	explicit permutational_mutation(double _quantity_coefficient)
		: quantity_coefficient(_quantity_coefficient) {}

	std::vector<T> operator() (const std::vector<T>& genome, double quantity) const
	{
		auto res = genome;

		double transposed_quantity = 1 - (1 - quantity) * 0.9;
		auto genes_to_permute = size_t(std::round(std::max(0., quantity_coefficient * transposed_quantity)));

		// std::cout << "Permuting " << genes_to_permute << " genes\n";

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
	std::optional<std::pair<double, double>> gene_values_restrictions = std::nullopt;

	modificational_mutation(
			double typical_gene_number, double typical_mutation_coefficient, std::optional<std::pair<double, double>> restrictions = std::nullopt)
									: typical_gene_mutation_number(typical_gene_number),
									typical_mutation_coefficient(typical_mutation_coefficient),
									gene_values_restrictions(std::move(restrictions))
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

			if (gene_values_restrictions) {
				res[gene_position] = std::clamp(res[gene_position], (*gene_values_restrictions).first, (*gene_values_restrictions).second);
			}
		}

		return res;
	}
};

// struct genome_generator {
//
//};

