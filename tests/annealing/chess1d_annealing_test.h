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

	inline void test_dynamic_error_computing() {
		chess1d_permutator permutator(3.);
		final_error_computer computer;

		// permutator.plug_mutation_controller(&computer);


		auto current_genome = generate_initial_chess_figure_positions(13);
		double current_error = computer.operator()(current_genome);
		for (li i = 0; i < 10; i++) {
			current_genome = permutator.operator()(current_genome, 0.7);
			auto info = permutator.get_last_mutation_descriptor();

			current_error = computer.operator()(current_genome, &info, current_error);

			display_chess_positioning(indexed_positions_to_matrix(current_genome));
			std::cout << current_genome << " : error: " << current_error
				<< ", error should be: " << base_queens_chess_error::compute(enumerate_chess_figures(current_genome))
			<< std::endl;
		}

	}

}