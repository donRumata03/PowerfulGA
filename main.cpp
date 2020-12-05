#include <tests/GA/old_GA_test.h>
#include <tests/GA/ga_test.h>
#include <tests/annealing/annealing_tests.h>
#include <problem_examples/chess_annealing/chess_annealing.h>
#include <tests/annealing/chess_operation_tests.h>

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


	launch_chess_queen_arranging(8);

	// test_generation_and_displaying();
	// test_chess_permutating();
	// test_chess_error_function();

	/// 									Performance tests:


	return 0;
}
