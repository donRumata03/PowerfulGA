//
// Created by Vova on 18.09.2020.
//

#pragma once

#include "GA/old_GA.h"
#include "tests/test_functions.h"

using namespace GA;

void matting_test()
{
	Genome g1 = { 2, 0 };
	Genome g2 = { -2, 0 };
	Genome g3 = { 3, 0 };


	Population p = { g1, g2, g3 };

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

	Parents parents = {{{ 1, 2, 3, 4}, { 6, 7, 8, 9}}};
	auto res = GA::perform_dummy_crossover_matting(parents);
	std::cout << res << std::endl;
}


void fancy_crossover_test()
{
	Genome parent1 = { 1, 3 };
	Genome parent2 = { 2, 6 };

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

	add_pairs_to_plot(smoothed, {});
	add_pairs_to_plot(smoothed2, {});
	std::cout << "Plotting!" << std::endl;

	show_plot();
}

void crossover_wrapper_test()
{
	Genome parent1 = { 1, 3 };
	Genome parent2 = { 2, 6 };

	Genome parent3 = { 6, 10 };
	Genome parent4 = { 6, 10 };


	LightParents p = {
			{&parent1, &parent2},
			{&parent3, &parent4},
	};

	// cout << (parent1[0] + parent2[0]) / 2 << " " << (parent1[1] + parent2[1]) / 2 << endl;

	normalizer normaaa(100000);

	for(size_t i = 0; i < 10; i++) std::cout << apply_crossover(p, mat_parents, normaaa) << std::endl;
	std::cout << std::endl;
	for(size_t i = 0; i < 10; i++) std::cout << apply_crossover(p, mat_parents, normaaa, crossover_mode::high_variance_genetic) << std::endl;

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
	Genome g1 = { 1, 4};
	Genome g2 = { 1, 5};
	Genome g3 = { 3, 6};
	Genome g4 = { 0, 7};
	Genome g5 = { -1, 4};

	LightPopulation pop = {
			&g1,
			&g2,
			&g3,
			&g4,
			&g5,
	};

	auto res = distribute_pairs(pop, 100, false /* true */);

	for (auto& p : res) std::cout << *p.first << " " << *p.second << std::endl;
}

void random_test()
{
	std::vector<size_t> vals;
	for (size_t i = 0; i < 10000; i++) vals.push_back(randint(0, 5));
	auto counter = make_Counter(vals);
	std::cout << counter << std::endl;
	pms to_plot;
	for (auto& pnt : get_sorted_elements(counter))
	{
		to_plot.emplace_back(double(pnt.first), double(pnt.second));
	}
	add_pairs_to_plot(to_plot, {});
	std::cout << "Plotting!" << std::endl;
	show_plot();
}



void new_gen_test()
{
	Population old_gen = {
			{1, 4},
			{7, 3},
			{1, 2},
			{0, 3},
			{1, 1.5},
			{0, 2}
	};

	normalizer normaaa(1000);

	std::vector<double> fitnesses;
	for (auto& gene : old_gen) fitnesses.push_back(parabaloid_fitness(gene));


	std::cout << "Fitnesses: " << fitnesses << std::endl;

	// cout << make_new_generation(old_gen, fitnesses, normaaa, 2, 0.3, 3) << endl;

}


