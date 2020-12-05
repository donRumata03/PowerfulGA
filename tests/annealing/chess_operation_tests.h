//
// Created by Vova on 05.12.2020.
//

#pragma once

#include "problem_examples/chess_annealing/chess_annealing.h"

inline void test_generation_and_displaying() {
	auto chess_pole = generate_initial_chess_figure_positions(8);

	std::cout << "Generated pole: " << chess_pole << "; Unpacked pole: " << split_into_pairs(chess_pole) << std::endl;
	display_chess_positioning(figure_set_to_matrix(split_into_pairs(chess_pole), 8));
}
