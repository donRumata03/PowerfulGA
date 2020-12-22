#include <tests/GA/old_GA_test.h>
#include <tests/GA/ga_test.h>
#include <tests/annealing/annealing_tests.h>
#include <problem_examples/chess_annealing/chess_annealing.h>
#include <tests/annealing/chess2d_operation_tests.h>
#include <tests/annealing/chess1d_annealing_test.h>

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


	std::vector<chess1d::total_chess_annealing_parameters> parameters = {
			{
				7.5,
				1.0
			},
			{
				10.,
				1.0
			},
			{
					12.5,
					1.0
			},
			{
				15.,
						1.0
			}


	};

	chess1d::test_chess_queen_arranging_for_parameters(50, 10'000, 50, parameters);
	// chess1d::launch_chess_queen_arranging(100);



	/// 								Path length optimization

	/// 									Performance tests:


	return 0;
}
