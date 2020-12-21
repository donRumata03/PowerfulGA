//
// Created by vova on 21.12.2020.
//


#pragma once

#include "problem_examples/chess_annealing/chess1d_annealing_genome_operationns.h"

namespace chess1d
{
	inline void test_chess_generation ()
	{
		auto genome = generate_initial_chess_figure_positions(8);

		std::cout << genome << std::endl;
		display_chess_positioning(indexed_positions_to_matrix(genome));
	}

	inline void test_simple_chess_mutation () {
		std::cout << "Before: " << std::endl;

		auto genome = generate_initial_chess_figure_positions(8);

		std::cout << genome << std::endl;
		display_chess_positioning(indexed_positions_to_matrix(genome));

		std::cout << "After: " << std::endl;

		chess1d_permutator permutator(10.);
		auto mutated = permutator.operator()(genome, 0.7);

		std::cout << mutated << std::endl;
		display_chess_positioning(indexed_positions_to_matrix(mutated));
	}

}