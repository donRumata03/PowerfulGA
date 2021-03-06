//
// Created by Vova on 20.12.2020.
//

#include <Annealing/annealing_optimize.h>
#include "path_minimizing.h"
#include "path_parsing_and_displaying.h"

std::vector<std::vector<bool>> get_default_matrix ()
{
	std::string input = *read_file(path_minimizing_data_path);
	// std::cout << "Parsing data: \n" << input << std::endl;

	return parse_way_matrix(input);
}

std::pair<std::vector<point>, double> minimize_path (const std::vector<std::vector<bool>>& way_matrix, size_t iterations)
{
	auto point_encoding = path_matrix_to_points(way_matrix);

	///
	auto path_length_counter = DummyPathLengthCounter(point_encoding);

	auto annealing_res = annealing_optimize<size_t, void>(
				[&path_length_counter](const std::vector<size_t>& point_permutation) -> double {
					return error_by_distance( path_length_counter(point_permutation) );
				},
				{
					.iterations = iterations,
					.exiting_value = 0.,
					.typical_temperature = 1.5,
					.genes_in_genome = point_encoding.size(),
					.resurrect_after_iterations = size_t(std::round(iterations * 0.1)),
				},
				generate_path,
				[](const std::vector<size_t>& genome, double amount) -> std::vector<size_t> {
					return mutate_path<size_t>(genome, amount);
				},
				custom_exp_temperature_dynamic(3.5)
			);
	auto[best_sequence, best_error] = std::tie(annealing_res.best_genome, annealing_res.best_energy);


	std::cout << best_sequence.size() << " " << best_sequence << std::endl;

	return { index_to_point_path(best_sequence, point_encoding), distance_by_error(best_error) };
}


/**
  * 8 minutes : 100'000'000 iterations
  * => 5 hours <=> 300 minutes : ≈ 100'000'000 * 37.5 iterations = 3'750'000'000 iterations
*/
void launch_path_minimizing (size_t iterations)
{
//	std::string input = *read_file(path_minimizing_data_path);
//	auto matrix = parse_way_matrix(input);
//	std::cout << matrix << std::endl;
	auto matrix = get_default_matrix();

	auto [best_points, best_distance] = minimize_path(matrix, iterations);

	std::cout
		<< "Optimized, best path length is: " << best_distance
		<< ", best error is: " << error_by_distance(best_distance)
		<< ", best score is: " << fitness_by_distance(best_distance)
	<< std::endl;

	std::cout << "________________________________________________________" << std::endl;
	std::cout << path_points_to_answer(best_points) <<  std::endl;

	write_file(path_points_to_answer(best_points), powerful_ga_base_dir / "problem_examples" / "path_minimizing" / "output.txt");

	add_points_to_plot(best_points);
	show_plot();
}
