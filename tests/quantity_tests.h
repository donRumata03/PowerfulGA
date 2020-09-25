//
// Created by Vova on 25.09.2020.
//

#pragma once


inline void test_quantity_counter_with_big_number() {
	size_t population_size = 100000;

	genome_quantity_orientation orientation = {
			.hazing_percent = 0.5,
			.algorithm_progress_percent = 0,

			.parent_fit_pow = 0.7,
			.usual_elite_fit_pow = 2,
			.hyper_elite_fit_pow = 5,

			.use_best_genome = true,
	};

	std::cout << "Initial value: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 0.5;

	std::cout << "Middle value #1: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 0.8;

	std::cout << "Middle value #2: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 1.;

	std::cout << "Finishing value: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;
}


inline void test_quantity_counter_with_little_number() {
	// size_t population_size = 10;
	size_t population_size = 3;

	genome_quantity_orientation orientation = {
			.hazing_percent = 0.5,
			.algorithm_progress_percent = 0,

			.parent_fit_pow = 0.7,
			.usual_elite_fit_pow = 2,
			.hyper_elite_fit_pow = 5,

			.use_best_genome = true,
	};

	std::cout << "Initial value: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 0.5;

	std::cout << "Middle value #1: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 0.8;

	std::cout << "Middle value #2: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;

	orientation.algorithm_progress_percent = 1.;

	std::cout << "Finishing value: " <<
	          calculate_genome_quantities(population_size, orientation)
	          << std::endl << std::endl;
}



inline void progress_coefficient_test() {
	size_t population_size = 100000;

	genome_quantity_orientation orientation = {
			.hazing_percent = 0.5,
			.algorithm_progress_percent = 0,

			.parent_fit_pow = 0.5,
			.usual_elite_fit_pow = 2,
			.hyper_elite_fit_pow = 5,

			.use_best_genome = true,
	};

	for (auto percent : linspace(0, 1, 50)) {
		orientation.algorithm_progress_percent = percent;
		/*std::cout << */calculate_genome_quantities(population_size, orientation);
	}
}


inline void test_quantity_dynamic_graphing() {
	size_t population_size = 100'000;

	genome_quantity_orientation orient =
		{
				.hazing_percent = 0.5,
				.algorithm_progress_percent = 0,
				.parent_fit_pow = 1,

				.usual_elite_fit_pow = 3,
				.hyper_elite_fit_pow = 5,
				.use_best_genome = true
		};

	view_quantity_dynamic(population_size, orient);
}
