//
// Created by Vova on 05.12.2020.
//

#pragma once

#include "problem_examples/chess_annealing/chess_annealing.h"

namespace chess2d
{
	inline void test_generation_and_displaying ()
	{
		auto chess_pole = chess2d::generate_initial_chess_figure_positions(8);

		std::cout << "Generated pole: " << chess_pole << "; Unpacked pole: " << split_into_pairs(chess_pole)
		          << std::endl;
		display_chess_positioning(figure_set_to_matrix(split_into_pairs(chess_pole), 8));
	}

	inline void test_chess_permutating ()
	{
		auto chess_pole = chess2d::generate_initial_chess_figure_positions(8);

		auto permutated = chess2d::chess_permutator(8, 4)(split_into_pairs(chess_pole), 0.5);
		// std::cout << permutated << std::endl;

		std::cout << "Initial pole: " << std::endl;
		display_chess_positioning(figure_set_to_matrix(split_into_pairs(chess_pole), 8));

		std::cout << "Permuted: " << std::endl;
		display_chess_positioning(figure_set_to_matrix(permutated, 8));
	}


	inline void test_chess_error_function ()
	{
		auto chess_pole = chess2d::generate_initial_chess_figure_positions(8);

		auto permutated = chess2d::chess_permutator(8, 4)(split_into_pairs(chess_pole), 1);
		// std::cout << permutated << std::endl;

		std::cout << "Initial pole: " << std::endl;
		display_chess_positioning(figure_set_to_matrix(split_into_pairs(chess_pole), 8));
		std::cout << "Error: " << chess2d::chess2d_error<are_queens_colliding>()(chess_pole) << std::endl;

		std::cout << "Permuted: " << std::endl;
		display_chess_positioning(figure_set_to_matrix(permutated, 8));
		std::cout << "Error: " << base_chess_error<are_queens_colliding>::compute(permutated) << std::endl;
	}
}