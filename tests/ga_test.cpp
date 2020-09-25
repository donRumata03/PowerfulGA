#include "GA/GA.h"
// #include "GA/binary_GA.h"
#include <iostream>

#include "local_testing.h"
#include "test_functions.h"

#include "ga_util_tests.h"

#include "quantity_tests.h"

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




void ga_test()
{
	renew_random();
	
	// pms ranges = { { -512, 512 }, { -512, 512 } };
	std::vector<std::pair<double, double>> ranges = { { -100, 100 }, { -100, 100 } };

	GA_params params;

	size_t total_computations = 500'000;
	params.population_size = 20'000;
	params.epoch_num = total_computations / params.population_size;

	params.set_default_epoch_num(1000'000);
	std::cout << params.population_size << " " << params.epoch_num << std::endl;


	params.hazing_percent = 0.7;
	
	params.target_gene_mutation_number = 0.1;
	params.mutation_percent_sigma = 0.01;
	params.cut_mutations = false;
	// params.best_genome_percent = 0.05;

	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.3;

	// Multithreading parameters:
	params.allow_multithreading = false;
	params.threads = 10;

	params.crossover_mode = crossover_mode::low_variance_genetic;

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

	params.hazing_percent = 0.6;

	params.target_gene_mutation_number = 0.1;
	params.mutation_percent_sigma = 0.01;
	params.cut_mutations = false;
	// params.best_genome_percent = 0.05;

	params.elite_fit_pow = 2;
	params.parent_fit_pow = 0.1;

	// Multithreading parameters:
	params.allow_multithreading = false;
	params.threads = 10;

	params.exiting_fitness_value = 1e+10;
	
	params.crossover_mode = crossover_mode::low_variance_genetic;

	auto callback = [](const double percent, const double best_fitness, const std::vector<double>& best_genome)
	{
		std::cout << "GA percent: " << percent << "; best function value now: " << 3 + 1 / best_fitness << "; best genome: " << best_genome << std::endl;
	};

	auto [opt, values] = ga_optimize([](const std::vector<double>& vals) { return 1 / abs((vals[0] * vals[0] - 4 * vals[0] + 7) - 3); }, ranges, params, callback);

	std::cout << "\n\n\nBest function value: " << (3 + 1 / opt) << " at parameters: " << values << std::endl;
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	system(("chcp " + std::to_string(CP_UTF8)).c_str());

	std::function<void(void)> value = test_GD;

	// std::cout << std::log(10) / std::log(2) << std::endl;

	/// 								Local optimization:
	// test_GD();
	// test_newton_optimize();
	// test_gradient_counting();
	// compare_GD_and_newton();

	/// 										GA itself:
	// simple_ga_test();


	// ga_test();
	/// 										GA operations:
	// fancy_crossover_test();
	// crossover_wrapper_test();
	// parent_choosing_test();
	// new_gen_test();

	/// Quantity:
	// test_quantity_counter_with_big_number();
	// test_quantity_counter_with_little_number();
	// progress_coefficient_test();
	test_quantity_dynamic_graphing();

	return 0;
}
