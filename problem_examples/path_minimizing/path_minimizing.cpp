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

std::pair<std::vector<point>, double> minimize_path (const std::vector<std::vector<bool>>& way_matrix)
{
	auto point_encoding = path_matrix_to_points(way_matrix);

	auto [best_sequence, best_error] = annealing_optimize<size_t, void>(

			);



	return { distance_by_error(best_error), index_to_point_path(best_sequence, point_encoding) };
}

void launch_path_minimizing ()
{
//	std::string input = *read_file(path_minimizing_data_path);
//	auto matrix = parse_way_matrix(input);
//	std::cout << matrix << std::endl;
	auto matrix = get_default_matrix();

	auto [best_points, best_error] = minimize_path(matrix);

	std::cout
		<< "Optimized, best path length is: " << distance_by_error(best_error)
		<< ", best score is " << fitness_by_distance(distance_by_error(best_error))
	<< std::endl;
}
