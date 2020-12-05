//
// Created by Vova on 03.12.2020.
//

#pragma once

#include <Annealing/annealing_optimize.h>

#include "tests/test_functions.h"

inline void complete_annealing_test() {
	auto answer = annealing_optimize<double>(
			schaffer_function,
			AnnealingOptimizeParameters{
				.iterations = 100000,
				.typical_temperature = 0.02,
				.genes_in_genome = 2,
			},
			[](size_t number){
				std::vector<double> res(number);
				std::generate(res.begin(), res.end(), []() { return 200 * (pythonic_random() - 0.5); });
				return res;
			},
			modificational_mutation(1.5, 20, {{-100, 100}}),
			exp_temperature_dynamic
	);

	std::cout << answer << std::endl;
}

/// Operations testing:

inline void annealing_permutational_mutation_test() {
	std::vector<double> test_values = {
			1, 2, 3, 4, 5
	};

	double typical_gene_mutation_number = 3;
	double quantity = 0.5;

	for (size_t i = 0; i < 100; ++i) {
		std::cout << permutational_mutation<double>(typical_gene_mutation_number)(test_values, i / 100.) << std::endl;
	}
}

inline void annealing_modificational_mutation_test() {

}


