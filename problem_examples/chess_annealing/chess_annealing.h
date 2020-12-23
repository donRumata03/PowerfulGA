//
// Created by Vova on 04.12.2020.
//

#pragma once

#include <ostream>
#include "Annealing/annealing_optimize.h"
#include "chess_annealing_utils.h"
#include "chess2d_annealing_genome_operations.h"


namespace chess1d
{
	std::optional<std::vector<li>> arrange_chess_queens (li n, size_t max_iterations, bool output_debug = true);

	void launch_chess_queen_arranging (li n);

	struct total_chess_annealing_parameters {
		double permutation_intensity {};
		double typical_temperature {};

		void print(std::ostream& os) const {
			os << "permutation_intensity: " << permutation_intensity << " typical_temperature: "
				<< typical_temperature;
		}
	};

	// Breaks everything!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	inline std::ostream& operator<< (std::ostream& os, const total_chess_annealing_parameters& parameters)
//	{
//		os << "permutation_intensity: " << parameters.permutation_intensity << " typical_temperature: "
//		   << parameters.typical_temperature;
//		return os;
//	}

	void launch_parameter_testing();

	void test_chess_queen_arranging_for_parameters(li n, size_t max_iterations, size_t repetitions, const std::vector<total_chess_annealing_parameters>& parameters);

	std::optional<std::vector<li>> launch_chess_annealing_with_automatic_iterations(li n);
	void output_python_code_below_n (li n, const std::string& output_file);
}


namespace chess2d
{
	void launch_chess_queen_arranging (li n);

	std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens (li n, size_t max_iterations);
}


