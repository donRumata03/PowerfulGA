//
// Created by Vova on 20.12.2020.
//

#include "path_minimizing.h"
#include "path_parsing_and_displaying.h"

std::vector<std::pair<li, li>> minimize_path (const std::vector<std::vector<bool>>& way_matrix)
{
	return std::vector<std::pair<li, li>>();
}

void launch_path_minimizing ()
{
	std::string input = *read_file<given_filename_encoding::utf8>(R"(D:\Projects\Utilities\GA\problem_examples\path_minimizing\statement_data.txt)");
	auto matrix = parse_way_matrix(input);
	std::cout << matrix << std::endl;

	auto res = minimize_path(matrix);

}
