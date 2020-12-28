//
// Created by Vova on 20.12.2020.
//

#include "path_minimizing.h"
#include "path_parsing_and_displaying.h"

std::vector<std::vector<bool>> get_default_matrix ()
{
	std::string input = *read_file(path_minimizing_data_path);
	// std::cout << "Parsing data: \n" << input << std::endl;

	return parse_way_matrix(input);
}

std::vector<std::pair<li, li>> minimize_path (const std::vector<std::vector<bool>>& way_matrix)
{
	return std::vector<std::pair<li, li>>();
}

void launch_path_minimizing ()
{
	std::string input = *read_file(path_minimizing_data_path);
	auto matrix = parse_way_matrix(input);
	std::cout << matrix << std::endl;

	auto res = minimize_path(matrix);

}
