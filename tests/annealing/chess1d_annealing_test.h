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

		display_chess_positioning(indexed_positions_to_matrix(genome));
	}


}