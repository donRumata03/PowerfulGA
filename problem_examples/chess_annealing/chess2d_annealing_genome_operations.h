//
// Created by Vova on 05.12.2020.
//

#pragma once

#include "chess_annealing_utils.h"
#include "problem_examples/chess_annealing/common_chess_genome_operationns.h"


namespace chess2d
{

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////				 Generation					////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

// template<class T>
	inline std::vector<li> generate_initial_chess_figure_positions (li n)
	{
		assert(!(n % 2));
		li figures = n / 2;

		std::vector<li> res(n);
		std::mt19937 gen(std::random_device {}());

		for (size_t i = 0; i < figures; ++i) {
			res[2 * i] = i;
			res[2 * i + 1] = randint(0, figures, gen);
		}

		return res;
	}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////				 Permutation				////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/// Returns the initial position if there are no empty ones
/// ROI: including corners
	inline std::pair<li, li> generate_free_position_around (
			std::pair<li, li> initial_position, const std::vector<std::vector<bool>>& matrix,
			std::pair<li, li> ROI_size)
	{

//	std::pair<li, li> real_corner = {
//			std::max(0LL, ROI_left_upper_corner.first - ROI_size.first),
//			std::max(0LL, ROI_left_upper_corner.second - ROI_size.second),
//	};
//	std::pair<li, li> right_

		li n = matrix.size();
		std::vector<std::pair<li, li>> possible_points;

		for (
				size_t i = std::max(0LL, initial_position.first - ROI_size.first);
				i <= std::min(initial_position.first + ROI_size.first, n - 1); ++i) {

			for (
					size_t j = std::max(0LL, initial_position.second - ROI_size.second);
					j <= std::min(initial_position.second + ROI_size.second, n - 1); ++j) {

				std::pair<li, li> this_point = { i, j };

				if (this_point != initial_position and not matrix[i][j]) {
					possible_points.push_back(this_point);
				}
			}
		}

		if (possible_points.empty()) return initial_position;
		return random_choice(possible_points, std::mt19937 { std::random_device {}() });
	}


	class chess_permutator
	{
		li n {};
		double permute_coefficient {};

	public:
		explicit chess_permutator (li _n, double _permute_coefficient)
				: n(_n),
				  permute_coefficient(_permute_coefficient)
		{}

		std::vector<std::pair<li, li>>
		operator() (const std::vector<std::pair<li, li>>& figure_positions, double amount) const
		{
			auto matrix = figure_set_to_matrix(figure_positions, n);
			auto res = figure_positions;
			std::shuffle(res.begin(), res.end(), std::mt19937 { std::random_device {}() });
			/// ^^^ Shuffle because we're deleting last {_x_} elements

			auto resultant_amount = li(std::round(amount * permute_coefficient));

			// Find the size of the zone of possible movement of the figures:
			li zone_size = resultant_amount / 2;

			std::vector<std::pair<li, li>> new_positions(resultant_amount);
			for (size_t mutation_index = 0; mutation_index < resultant_amount; ++mutation_index) {
				auto[old_x, old_y] = res.back();

				auto[new_x, new_y] = generate_free_position_around(res.back(), matrix, { zone_size, zone_size });

				// Delete the old one:
				matrix[old_x][old_y] = false;
				res.pop_back();

				// Add the new one:
				matrix[new_x][new_y] = true;
				new_positions[mutation_index] = { new_x, new_y };
			}

			std::copy(new_positions.begin(), new_positions.end(), std::back_inserter(res));

			// return matrix_to_figure_set(matrix);
			return res;
		}
	};


/////////////////////////////////

	template <class AreBeatingPredicate>
	struct chess2d_error
	{
		double operator() (const std::vector<li>& queen_positioning_data) const
		{
			return base_chess_error<AreBeatingPredicate>::compute(split_into_pairs(queen_positioning_data));
		}
	};
}