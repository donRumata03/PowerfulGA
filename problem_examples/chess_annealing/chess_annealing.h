//
// Created by Vova on 04.12.2020.
//

#pragma once

#include "Annealing/annealing_optimize.h"
#include "chess_annealing_utils.h"
#include "chess_annealing_genome_operations.h"



// using li = long long int;

namespace chess2d
{
	void launch_chess_queen_arranging (li n);

	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations);
}


