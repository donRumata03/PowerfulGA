//
// Created by vova on 25.12.2020.
//


#pragma once

#include <problem_examples/path_minimizing/path_parsing_and_displaying.h>
#include <problem_examples/path_minimizing/path_minimizing_utils.h>
#include <problem_examples/path_minimizing/path_minimizing.h>


inline void test_opening_and_parsing() {
	std::string input = *read_file(path_minimizing_data_path);
	std::cout << "Parsing data: \n" << input << std::endl;


	auto matrix = parse_way_matrix(input);
	std::cout << matrix << std::endl;

}

