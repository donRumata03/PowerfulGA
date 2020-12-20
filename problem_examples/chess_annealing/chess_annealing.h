//
// Created by Vova on 04.12.2020.
//

#pragma once

#include "Annealing/annealing_optimize.h"
#include "chess_annealing_utils.h"
#include "chess2d_annealing_genome_operations.h"


namespace chess1d
{
	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations);

	void launch_chess_queen_arranging (li n);
	void output_python_code_below(li n);
}


namespace chess2d
{
	void launch_chess_queen_arranging (li n);

	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations);
}


