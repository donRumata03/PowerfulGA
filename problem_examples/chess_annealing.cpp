//
// Created by Vova on 04.12.2020.
//

#include "chess_annealing.h"


std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens ()
{

}

void launch_chess_queen_arranging (li n)
{
	auto res = arrange_chess_queens();

	if (!res) {
		std::cout << "Didn't find any solutions!" << std::endl;
		return;
	}

	std::cout << "Found a solution (!):" << std::endl;
	display_chess_positioning(*res, n);
}
