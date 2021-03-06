//
// Created by Vova on 04.12.2020.
//

#include "chess_annealing.h"
#include "chess1d_annealing_genome_operationns.h"
#include "problem_examples/path_minimizing/path_minimizing_utils.h"

namespace chess1d
{

	std::optional<std::vector<li>>
	arrange_chess_queens (li n, size_t max_iterations, bool output_debug, std::vector<double> *for_usual_fitness,
	                      std::vector<double> *for_best_fitness, double *for_res,
	                      std::vector<double> *for_vague_fitness)
	{

		/*
		 * f(0.9) = 1
		 * f(0) is told
		 */

		double intensiveness_factor = n * 3. / 200.;
		double permutation_pow = std::log(intensiveness_factor) / 0.9;
		if (output_debug) {
			std::cout << "Permutation pow: " << permutation_pow << "; Intensiveness: " << intensiveness_factor << std::endl;
		}


		final_error_computer error_computer;
		chess1d_permutator permutator(intensiveness_factor, permutation_pow);
		// permutator.plug_mutation_controller(&error_computer);

		auto annealing_res = annealing_optimize<li, chess1d_permutator::mutation_descriptor>(
				error_computer,
				AnnealingOptimizeParameters {
						.iterations = max_iterations,
						.exiting_value = 0,
						.typical_temperature = n * 0.2 / 200., // ::transfer_range(double(n), { 0., 200. }, { 1., 3.5 }),
						.genes_in_genome = static_cast<size_t>(n),
						.resurrect_after_iterations = size_t(std::round(max_iterations * 0.1)),
				},
				generate_initial_chess_figure_positions,
				permutator,
				custom_exp_temperature_dynamic(9.),
				output_debug,
				for_vague_fitness,
				for_usual_fitness,
				for_best_fitness
		);
		auto[best_res, best_error] = std::tie(annealing_res.best_genome, annealing_res.best_energy);

		if(for_res) {
			*for_res = best_error;
		}

		if (best_error != 0) {
			if (output_debug) {
				std::cout << "Can't find a solution, but here's the best of the found ones (error: " << best_error
				          << "):"
				          << std::endl;

				if (n <= 20) display_chess_positioning(indexed_positions_to_matrix(best_res));
			}
			return std::nullopt;
		}

		return best_res;
	}

	void launch_chess_queen_arranging (li n)
	{
		std::cout << "Trying to get answer for n = " << n << std::endl;

		auto res = arrange_chess_queens(n, 200'000, true, nullptr, nullptr, nullptr, nullptr);

		if (!res) {
			std::cout << "Didn't find any solutions!" << std::endl;
			return;
		}

		std::cout << "Found a solution (!):" << std::endl;
		if (n <= 20) {
			display_chess_positioning(indexed_positions_to_matrix(*res));
		}
	}



	void test_chess_queen_arranging_for_parameters (li n, size_t max_iterations, size_t repetitions,
	                                                const std::vector<total_chess_annealing_parameters>& parameters)
	{
		std::cout << "N: " << n << ", iterations: " << max_iterations << std::endl;

		std::vector<size_t> res;

		for (auto& parameter_set : parameters) {
			std::cout << console_colors::yellow << "______________________________" << console_colors::remove_all_colors << std::endl;

			size_t successful_tries = 0;
			for (size_t i = 0; i < repetitions; ++i) {
				auto this_res = arrange_chess_queens(n, max_iterations, false, nullptr, nullptr, nullptr, nullptr);

				if (this_res) successful_tries++;
			}
			res.push_back(successful_tries);
		}

		std::cout << console_colors::green << "______________________________" << console_colors::remove_all_colors << std::endl;

		for (size_t i = 0; i < parameters.size(); ++i) {
			auto& parameter_set = parameters[i];
			auto successful_tries = res[i];

			std::cout << "Trying parameter set {";
			parameter_set.print(std::cout);
			std::cout << "}: ";

			std::cout << "Successful tries: " << successful_tries << " of " << repetitions << std::endl;
		}
	}

	void launch_parameter_testing ()
	{

		std::vector<chess1d::total_chess_annealing_parameters> parameters_for_20 = {

				{
						4.,
						0.1
				},
				{
						4.,
						0.2
				},
				{
						4.,
						0.3
				},
				{
						4.,
						0.5
				},
				{
						4.,
						1.0
				},
				{
						4.,
						2.0
				},
				{
						4.,
						3.0
				},

		};

		// {
		//						10.,
		//						1.0
		//				},
		// Are ≈ optimal for 50
		std::vector<chess1d::total_chess_annealing_parameters> parameters_for_50 = {

				{
						10.,
						0.3
				},
				{
						10.,
						0.5
				},
				{
						10.,
						1.0
				},
				{
						10.,
						2.0
				},
				{
						10.,
						3.0
				},

		};



		std::vector<chess1d::total_chess_annealing_parameters> parameters_for_100 = {
				{
						20.,
						0.44
				},
				{
						20.,
						0.66
				},
				{
						20.,
						1.0
				},
				{
						20.,
						1.5
				},
				{
						20.,
						2.0
				},
				{
						20.,
						3.0
				},
		};


		//              {
		//						26.,
		//						2.0
		//				},
		// Are optimal for 130

		std::vector<chess1d::total_chess_annealing_parameters> parameters_for_130 = {
//				{
//						26.,
//						0.44
//				},
				{
						26.,
						1.0
				},
				{
						26.,
						2.0
				},
//				{
//						26.,
//						3.0
//				},
		};

		std::vector<chess1d::total_chess_annealing_parameters> parameters_for_200 = {
//				{
//						40.,
//						1.0
//				},
//				{
//						40.,
//						2.0
//				},
//				{
//						26.,
//						2.5
//				},
				{
						26.,
						3.0
				},
				{
						26.,
						4.0
				},
				{
						32.,
						6.0
				},
				{
						40.,
						10.0
				},

		};


		size_t n = 200;
		auto &parameters = parameters_for_200;
		size_t iterations = 500'000;
		size_t repetitions = 7;

		chess1d::test_chess_queen_arranging_for_parameters(n, iterations, repetitions, parameters);

	}

	size_t get_default_iterations (li n)
	{
		li iterations = li(std::round(transfer_range(double(n), { 0., 200. }, { 20'000, 500'000 })));
		return size_t(std::max(iterations, 0LL));
	}

	std::optional<std::vector<li>> launch_chess_annealing_with_automatic_iterations (li n)
	{
		li iterations = get_default_iterations(n); // transfer_range(double(n), { 0., 200. }, { 20'000, 500'000 });
		return arrange_chess_queens(n, iterations, false, nullptr, nullptr, nullptr, nullptr);
	}

	void output_python_code_below_n (li n, const std::string& output_file)
	{
		std::ofstream output_stream;
		if (!output_file.empty()) {
			output_stream.open(output_file);
		}

		for (li i = 4; i <= n; ++i) {
			bool success = false;

			while(!success) {
				auto this_try = launch_chess_annealing_with_automatic_iterations(i);
				success = bool(this_try);
				if (success) {
					std::cout << i << " : " << *this_try << ", " << std::endl;
					output_stream << i << " : " << *this_try << ", " << std::endl;
				}
			}
		}

		output_stream.close();
	}

	void visualize_fitness_dynamic (li n)
	{
		size_t iterations = 100'000; // get_default_iterations(n);

		double for_err = -1;
		std::vector<double> for_usual_fitness;
		std::vector<double> for_best_fitness;
		std::vector<double> for_vague_fitness;

		for_usual_fitness.reserve(iterations);
		for_best_fitness.reserve(iterations);
		for_vague_fitness.reserve(iterations);

		std::cout << "Arranging " << n << " chess queens (" << iterations << " iterations)…";
		auto res = arrange_chess_queens(n, iterations, true, &for_usual_fitness, &for_best_fitness, &for_err, &for_vague_fitness);
		std::cout << "Done!" << std::endl;
		if (res) {
			std::cout << console_colors::green << "Finished SUCCESSFULLY!" << console_colors::remove_all_colors << std::endl;
		}
		else {
			// final_error_computer computer;

			std::cout << console_colors::red <<
				"Finished with error function in the end: " << for_err << "!"
			<< console_colors::remove_all_colors << std::endl;
		}

		auto usual_enumerated = enumerate<double>(for_usual_fitness);
		auto best_enumerated = enumerate<double>(for_best_fitness);
		auto vague_enumerated = enumerate<double>(for_vague_fitness);

		add_pairs_to_plot(vague_enumerated, { .name = "Vague Error Dynamic" });
		add_pairs_to_plot(usual_enumerated, { .name = "Error Dynamic" });
		add_pairs_to_plot(best_enumerated, { .name = "Best Error Dynamic" });


		show_plot();
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

		auto annealing_res = annealing_optimize<li, void>(
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
				default_exp_temperature_dynamic
		);
		auto[best_res, best_error] = std::tie(annealing_res.best_genome, annealing_res.best_energy);


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