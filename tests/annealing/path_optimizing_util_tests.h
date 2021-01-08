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

inline void test_distance_generation() {
	auto size = 513;
	double amount = 0.;

	for (size_t i = 0; i < 20; ++i) {
		std::cout << distance_by_amount(amount, size) << std::endl;
	}
}


inline void test_reversing_mutation_full_path() {
	auto points = path_matrix_to_points(get_default_matrix());

	auto dist = distance_by_amount(0.5, points.size());
	std::cout << "Distance is: " << dist << std::endl;

	std::cout << "Before mutation: " << points << std::endl;
	add_points_to_plot(points, { .type = "plot" });

	reversing_mutate(points, dist);

	std::cout << "After mutation: " << points << std::endl;
	add_points_to_plot(points, { .type = "plot" });

	show_plot();

}

inline void test_reversing_mutation_short_path() {
	// auto points = path_matrix_to_points(get_default_matrix());
	std::vector<size_t> indexes(10);
	std::iota(indexes.begin(), indexes.end(), 0);

	auto dist = distance_by_amount(0.5, indexes.size());
	std::cout << "Distance is: " << dist << std::endl;

	std::cout << "Before mutation: " << indexes << std::endl;

	reversing_mutate(indexes, dist);

	std::cout << "After mutation: " << indexes << std::endl;
}

/// ################################################################################


inline void test_moving_mutation_full_path() {
	auto points = path_matrix_to_points(get_default_matrix());

	auto dist = distance_by_amount(0.5, points.size());
	std::cout << "Distance is: " << dist << std::endl;

	std::cout << "Before mutation: " << points << std::endl;
	add_points_to_plot(points, { .type = "plot" });

	moving_mutate(points, dist);

	std::cout << "After mutation: " << points << std::endl;
	add_points_to_plot(points, { .type = "plot" });

	show_plot();
}


inline void test_moving_mutation_short_path() {
	std::vector<size_t> indexes(10);
	std::iota(indexes.begin(), indexes.end(), 0);

	auto dist = distance_by_amount(0.5, indexes.size());
	std::cout << "Distance is: " << dist << std::endl;

	std::cout << "Before mutation: " << indexes << std::endl;

	moving_mutate(indexes, dist);

	std::cout << "After mutation: " << indexes << std::endl;
}


inline void test_path_to_answer_converting() {
	// TODO
}
