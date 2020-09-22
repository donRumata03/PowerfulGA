//
// Created by Vova on 18.09.2020.
//

#pragma once

#include "GA/GA.h"

using namespace GA;

void matting_test()
{
	genome g1 = { 2, 0 };
	genome g2 = { -2, 0 };
	genome g3 = { 3, 0 };


	population p = { g1, g2, g3 };

	std::vector<double> fit = { 1 / parabaloid(g1), 1 / parabaloid(g2), 1 / parabaloid(g3) };
	std::cout << fit << std::endl;

	std::cout << GA::select_matting_pool(p, fit, 10, false) << std::endl;

}

void gen_test()
{
	std::vector<std::pair<double, double>> ranges = { {1, 2}, {2, 3}, { -1, 0 } };
	auto pop = generate_population(ranges, 10);
	std::cout << pop << std::endl;
}


void crossover_test()
{
	srand(unsigned(time(nullptr)));
	renew_random();

	parents_t parents = {{{1, 2, 3, 4}, {6, 7, 8, 9}}};
	auto res = GA::perform_dummy_crossover_matting(parents);
	std::cout << res << std::endl;
}


void fancy_crossover_test()
{
	genome parent1 = { 1, 3 };
	genome parent2 = { 2, 6 };

	// cout << (parent1[0] + parent2[0]) / 2 << " " << (parent1[1] + parent2[1]) / 2 << endl;

	normalizer normaaa(100000);

	std::vector<double> values1, values2;
	for (size_t i = 0; i < 100000; i++) {
		auto p = mat_parents({ &parent1, &parent2 }, normaaa, crossover_mode::high_variance_genetic);
		values1.push_back(p[0]);
		values2.push_back(p[1]);
	}

	auto smoothed = count_number_density(values1, 0.02, 1000, 1);
	auto smoothed2 = count_number_density(values2, 0.02, 1000, 1);

	add_to_plot(smoothed, {});
	add_to_plot(smoothed2, {});
	std::cout << "Plotting!" << std::endl;

	show_plot();
}

void crossover_wrapper_test()
{
	genome parent1 = { 1, 3 };
	genome parent2 = { 2, 6 };

	genome parent3 = { 6, 10 };
	genome parent4 = { 6, 10 };


	light_parents_t p = {
			{&parent1, &parent2},
			{&parent3, &parent4},
	};

	// cout << (parent1[0] + parent2[0]) / 2 << " " << (parent1[1] + parent2[1]) / 2 << endl;

	normalizer normaaa(100000);

	for(size_t i = 0; i < 10; i++) std::cout << launch_crossover(p, normaaa) << std::endl;
	std::cout << std::endl;
	for(size_t i = 0; i < 10; i++) std::cout << launch_crossover(p, normaaa, crossover_mode::high_variance_genetic) << std::endl;

	/*
	vector<double> values1, values2;
	for (size_t i = 0; i < 100000; i++) {
		auto p = mat_parents({ &parent1, &parent2 }, normaaa, crossover_mode::high_variance_genetic);
		values1.push_back(p[0]);
		values2.push_back(p[1]);
	}
	*/
}

void parent_choosing_test()
{
	genome g1 = {1, 4};
	genome g2 = {1, 5};
	genome g3 = {3, 6};
	genome g4 = {0, 7};
	genome g5 = {-1, 4};

	light_population pop = {
			&g1,
			&g2,
			&g3,
			&g4,
			&g5,
	};

	auto res = distribute_pairs(pop, 100, false /* true */);

	for (auto& p : res) std::cout << *p.first << " " << *p.second << std::endl;
}




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
	size_t population_size = 10;

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

			.parent_fit_pow = 0.7,
			.usual_elite_fit_pow = 2,
			.hyper_elite_fit_pow = 5,

			.use_best_genome = true,
	};

	for (auto percent : linspace(0, 1, 50)) {
		orientation.algorithm_progress_percent = percent;
		/*std::cout << */calculate_genome_quantities(population_size, orientation);
	}
}
