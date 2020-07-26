#include "GA/GA.h"
// #include "GA/binary_GA.h"
#include <iostream>

#include "local_testing.h"
#include "test_functions.h"

using namespace GA;
// using namespace binary_GA;

double parabaloid(std::vector<double>& xs)
{
	double res = 0;
	for (double x : xs) res += x * x;
	return res;
}


double parabaloid_fitness(std::vector<double>& xs)
{
	return 1 / parabaloid(xs);
}



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
	auto res = GA::perform_crossover_matting(parents);
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
		auto p = mat_parents({ &parent1, &parent2 }, normaaa, matting_mode::high_variance_genetic);
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

	for(size_t i = 0; i < 10; i++) std::cout << perform_crossover(p, normaaa) << std::endl;
	std::cout << std::endl;
	for(size_t i = 0; i < 10; i++) std::cout << perform_crossover(p, normaaa, matting_mode::high_variance_genetic) << std::endl;
	
	/*
	vector<double> values1, values2;
	for (size_t i = 0; i < 100000; i++) {
		auto p = mat_parents({ &parent1, &parent2 }, normaaa, matting_mode::high_variance_genetic);
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

void ga_test()
{
	renew_random();
	
	// pms ranges = { { -512, 512 }, { -512, 512 } };
	std::vector<std::pair<double, double>> ranges = { { -100, 100 }, { -100, 100 } };

	GA_params params;

	size_t total_computations = 5000000;
	params.population_size = 500000;
	params.epoch_num = total_computations / params.population_size;
	
	params.hazing_percent = 0.3;
	
	params.target_gene_mutation_number = 0.1;
	params.mutation_percent_sigma = 0.01;
	params.cut_mutations = false;
	params.best_genome_percent = 0.05;

	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.1;

	// Multithreading parameters:
	params.allow_multithreading = true;
	params.threads = 10;

	params.mode_of_matting = matting_mode::low_variance_genetic;

	std::cout << std::thread::hardware_concurrency() << " threads" << std::endl;

	// std::function<void(double, double, const vector<double>&)> callback = [];
	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best error now: " << 0.29 + 1 / best_fitness << "; best genome: " << best_genome << std::endl;
	};
	
	auto [opt, values] = ga_optimize(shaffer_fit, ranges, params, callback);

	std::cout << "\n\n\nBest function value: " << (0.29 + 1 / opt) << " at parameters: " << values << std::endl;
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
	add_to_plot(to_plot, {});
	std::cout << "Plotting!" << std::endl;
	show_plot();
}



void new_gen_test()
{
	population old_gen = {
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

void simple_ga_test()
{
	renew_random();

	// pms ranges = { { -512, 512 }, { -512, 512 } };
	std::vector<std::pair<double, double>> ranges = { { -3, 3 } };

	GA_params params;

	size_t total_computations = 100;
	params.population_size = 20;
	params.epoch_num = total_computations / params.population_size;

	params.hazing_percent = 0.3;

	params.target_gene_mutation_number = 0.1;
	params.mutation_percent_sigma = 0.01;
	params.cut_mutations = false;
	params.best_genome_percent = 0.05;

	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.1;

	// Multithreading parameters:
	params.allow_multithreading = false;
	params.threads = 10;

	params.exiting_fitness_value = 1e+10;
	
	params.mode_of_matting = matting_mode::low_variance_genetic;

	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best function value now: " << 3 + 1 / best_fitness << "; best genome: " << best_genome << std::endl;
	};

	auto [opt, values] = ga_optimize([](const std::vector<double>& vals) { return 1 / abs((vals[0] * vals[0] - 4 * vals[0] + 7) - 3); }, ranges, params, callback);

	std::cout << "\n\n\nBest function value: " << (3 + 1 / opt) << " at parameters: " << values << std::endl;
}

int main()
{
	test_GD();
	// test_newton_optimize();
	// test_gradient_counting();
	// compare_GD_and_newton();
	
	// simple_ga_test();

	// fancy_crossover_test();

	// crossover_wrapper_test();

	// parent_choosing_test();

	// new_gen_test();
	
	return 0;
}
