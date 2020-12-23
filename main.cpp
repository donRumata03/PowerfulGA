#include <tests/GA/old_GA_test.h>
#include <tests/GA/ga_test.h>
#include <tests/annealing/annealing_tests.h>
#include <problem_examples/chess_annealing/chess_annealing.h>
#include <tests/annealing/chess2d_operation_tests.h>
#include <tests/annealing/chess1d_annealing_test.h>
#include <problem_examples/path_minimizing/path_minimizing_utils.h>

int main()
{
	set_utf8_in_console();

	/// 								Local optimization:
	// test_GD();
	// test_newton_optimize();
	// test_gradient_counting();
	// compare_GD_and_newton();


	/// 										GA itself:
	// old_ga_test();
	// test_GA();

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
	chess1d::output_python_code_below_n(200, R"(D:\Projects\Utilities\GA\problem_examples\chess_annealing\output_data.txt)");


	/// 								Path length optimization

	/// 									Performance tests:


	return 0;
}
