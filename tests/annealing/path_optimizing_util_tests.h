//
// Created by vova on 25.12.2020.
//


#pragma once

#include <problem_examples/path_minimizing/path_parsing_and_displaying.h>
#include <problem_examples/path_minimizing/path_minimizing_utils.h>
#include <problem_examples/path_minimizing/path_minimizing.h>


inline void test_opening_and_parsing() {
	auto matrix = get_default_matrix();
	std::cout << matrix << std::endl;

	std::cout << "Points (" << path_matrix_to_points(matrix).size() << "): " << path_matrix_to_points(matrix) << std::endl;
}

inline void test_visualizing() {
	auto points = path_matrix_to_points(get_default_matrix());

	add_points_to_plot(points, { .type = "scatter" });
	show_plot();
}

inline void test_range_generation() {
	size_t point_number = 10;
	size_t distance = 2;

	for (size_t i = 0; i < 20; ++i) {
		std::cout << generate_range(point_number, distance) << std::endl;
	}
}

inline void test_reversing_mutation() {
	auto points = path_matrix_to_points(get_default_matrix());



	add_points_to_plot(points, { .type = "scatter" });
	show_plot();

}

inline void test_moving_mutation() {

}


inline void test_path_to_answer_converting() {
	// TODO
}
