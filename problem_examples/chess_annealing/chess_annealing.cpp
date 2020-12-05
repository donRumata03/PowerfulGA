//
// Created by Vova on 04.12.2020.
//

#include "chess_annealing.h"


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

	auto [best_res, best_error] = annealing_optimize<li>(
			estimate_queens_chess_error(n),
			AnnealingOptimizeParameters {
				.iterations = max_iterations,
				.exiting_value = 0,
				.typical_temperature = 1.,
				.genes_in_genome = static_cast<size_t>(n * 2),
			},
			generate_initial_chess_figure_positions,
			[&](const auto& genome, double amount) {
				return unpack_pairs(chess_permutator(n, 3)(split_into_pairs(genome), amount));
			},
			exp_temperature_dynamic
			);

	// double error = best_error;
	std::vector<std::pair<li, li>> decompressed_res = split_into_pairs(best_res);

	if (best_error != 0) {
		std::cout << "Can't find a solution, but here's the best of the found ones:" << std::endl;
		display_chess_positioning(figure_set_to_matrix(decompressed_res, n));
		return {};
	}

	return decompressed_res;
}

void launch_chess_queen_arranging (li n)
{
	auto res = arrange_chess_queens(n, 1000);

	if (!res) {
		std::cout << "Didn't find any solutions!" << std::endl;
		return;
	}

	std::cout << "Found a solution (!):" << std::endl;
	display_chess_positioning(figure_set_to_matrix(*res, n));
}
