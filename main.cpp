#include "powerful_ga_config.h"

#include <tests/GA/old_GA_test.h>
#include <tests/GA/ga_test.h>
#include <tests/annealing/annealing_tests.h>
#include <problem_examples/chess_annealing/chess_annealing.h>
#include <tests/annealing/chess2d_operation_tests.h>
#include <tests/annealing/chess1d_annealing_test.h>
#include <problem_examples/path_minimizing/path_minimizing_utils.h>
#include <tests/annealing/path_optimizing_util_tests.h>
#include <tests/benchmarking/GA_adapter_test.h>
#include <tests/computation_distribution_test.h>


int main()
{
	set_utf8_in_console();
	srand(std::random_device{}());

	/// 								Local optimization:
	// test_GD();
	// test_newton_optimize();
	// test_gradient_counting();
	// compare_GD_and_newton();


	/// 										GA itself:
	// old_ga_test();
	// test_GA();
	// run_GA_with_iteration_distribution();

	/// 										GA operations:
	// fancy_crossover_test();
	// crossover_wrapper_test();
	// parent_choosing_test();
	// new_gen_test();

	/// 											GA Quantity:
	// test_quantity_counter_with_big_number();
	// test_quantity_counter_with_little_number();
	// progress_coefficient_test();
	// test_quantity_dynamic_graphing();

	/// 									GA computation distribution

	// test_averages();
	explore_performance_vs_epoch_pow();


	/// 										Annealing:
	// annealing_permutational_mutation_test();
	// complete_annealing_test();

	/// 										Chess 2d

	// chess2d::test_generation_and_displaying();
	// chess2d::test_chess_permutating();
	// chess2d::test_chess_error_function();

	// chess2d::launch_chess_queen_arranging(8);

	///										Chess 1d
	// Tests

	// chess1d::test_chess_generation ();
	// chess1d::test_simple_chess_mutation ();
	// chess1d::test_dynamic_error_computing();


	// chess1d::launch_parameter_testing();
	// chess1d::launch_chess_queen_arranging(200);

	// std::cout << *chess1d::launch_chess_annealing_with_automatic_iterations(150) << std::endl;
	// chess1d::output_python_code_below_n(200, R"(D:\Projects\Utilities\GA\problem_examples\chess_annealing\output_data.txt)");
	// chess1d::visualize_fitness_dynamic (500);


	/// 								Path length optimization

	// test_opening_and_parsing();
	// test_visualizing();

	// test_range_generation();
	// test_distance_generation();

	// test_reversing_mutation_full_path();
	// test_reversing_mutation_short_path();
	// ____________________________________
	// test_moving_mutation_full_path();
	// test_moving_mutation_short_path();

	// test_path_to_answer_converting();

	// launch_path_minimizing(100'000'000); /// 8 minutes
	// launch_path_minimizing(3'750'000'000); /// 5 hours



	/// 									Performance tests:

	// GA_adapter_test();

	// std::cout << distribute_computations_defaultly(1000000) << std::endl;

	return 0;
}
