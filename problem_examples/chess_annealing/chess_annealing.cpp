//
// Created by Vova on 04.12.2020.
//

#include "chess_annealing.h"


namespace chess1d
{

	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations)
	{
		return std::optional<std::vector<std::pair<li, li>>>();
	}

	void launch_chess_queen_arranging (li n)
	{

	}

	void output_python_code_below (li n)
	{

	}
}



namespace chess2d
{

	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations)
	{
//			[&](size_t number){
//				std::vector<double> res(number);
//				std::iota(res.begin(), res.end(), 0);
//				// std::generate(res.begin(), res.end(), []() { return ; });
//				std::random_device rd;
//				std::mt19937 g(rd());
//				std::shuffle(res.begin(), res.end(), g);
//				return res;
//			},

		auto[best_res, best_error] = annealing_optimize<li>(
				chess2d_error<are_queens_colliding>(),
				AnnealingOptimizeParameters {
						.iterations = max_iterations,
						.exiting_value = 0,
						.typical_temperature = 6,
						.genes_in_genome = static_cast<size_t>(n * 2),
				},
				generate_initial_chess_figure_positions,
				[&] (const auto& genome, double amount) {
					return unpack_pairs(chess_permutator(n, 6.0)(split_into_pairs(genome), amount));
				},
				exp_temperature_dynamic
		);

		// double error = best_error;
		std::vector<std::pair<li, li>> decompressed_res = split_into_pairs(best_res);

		if (best_error != 0) {
			std::cout << "Can't find a solution, but here's the best of the found ones (error: " << best_error << "):"
			          << std::endl;
			display_chess_positioning(figure_set_to_matrix(decompressed_res, n));
			return {};
		}

		return decompressed_res;
	}

	void launch_chess_queen_arranging (li n)
	{
		auto res = arrange_chess_queens(n, 500'000);

		if (!res) {
			std::cout << "Didn't find any solutions!" << std::endl;
			return;
		}

		std::cout << "Found a solution (!):" << std::endl;
		display_chess_positioning(figure_set_to_matrix(*res, n));
	}
}