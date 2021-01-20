//
// Created by Vova on 18.12.2020.
//

#pragma once

#include "problem_examples/chess_annealing/chess_annealing_utils.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////				 Error computing			////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


template<class AreBeatingPredicate>
struct base_chess_error
{
	static_assert(std::is_same_v<decltype(
	              AreBeatingPredicate::compute(
			              std::pair<li, li>(), std::pair<li, li>())
	              ), bool>,
	              "False AreBeatingPredicate's return type"
	);

	static double compute(const std::vector<std::pair<long long, long long>>& queen_positions) {
		size_t beating_count = 0;
		for (size_t first = 0; first < queen_positions.size(); ++first) {
			for (size_t second = first + 1; second < queen_positions.size(); ++second) {
				if (AreBeatingPredicate::compute(queen_positions[first], queen_positions[second])) {
					beating_count++;
				}
			}
		}
		return beating_count;
	}
};



struct are_queens_colliding {
	static bool compute(std::pair<li, li> q1, std::pair<li, li> q2) {
		auto [x1, y1] = q1;
		auto [x2, y2] = q2;

		if (x1 == x2 || y1 == y2) {
			return true;
		}
		li dx = x2 - x1;
		li dy = y2 - y1;
		if (std::abs(dx) == std::abs(dy)) {
			return true;
		}

		return false;
	}
};

struct are_horses_colliding {
	static bool compute(std::pair<li, li> h1, std::pair<li, li> h2) {
		auto [x1, y1] = h1;
		auto [x2, y2] = h2;

		li dx = x2 - x2;
		li dy = y1 - y2;

		if (
				(std::abs(dx) == 2 and std::abs(dy) == 3)
				or
				(std::abs(dx) == 3 and std::abs(dy) == 2)
				)
		{
			return true;
		}

		return false;
	}
};

struct are_slonopotams_colliding {
	static bool compute(std::pair<li, li> s1, std::pair<li, li> s2) {
		return are_horses_colliding::compute(s1, s2) || are_queens_colliding::compute(s1, s2);
	}
};

////////////////////////////////////////////

using base_queens_chess_error = base_chess_error<are_queens_colliding>;
using base_horses_chess_error = base_chess_error<are_horses_colliding>;
using base_slonopotams_chess_error = base_chess_error<are_slonopotams_colliding>;




