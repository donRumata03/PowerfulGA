//
// Created by Vova on 18.12.2020.
//

#pragma once

#include "chess_annealing_utils.h"
#include "common_chess_genome_operationns.h"



namespace chess1d
{
	inline std::vector<li> generate_initial_chess_figure_positions (li n)
	{
		std::vector<li> res(n);
		std::mt19937 gen(std::random_device {}());

		std::generate(res.begin(), res.end(), [n, &gen] () { return randint(0, n, gen); });

		return res;
	}

	inline void mutate_one (std::vector<li>& positions, size_t steps)
	{

	}

	template <class AreCollidingPredicate>
	class base_error_computer
	{
		// explicit base_error_computer(const std::vector<li>& initial_genome) {};

		virtual void experience_mutation (size_t figure_index, li new_position) = 0;

		[[nodiscard]] virtual li get_beating_amount () const = 0;
	};


	template <class AreCollidingPredicate>
	class error_computer_O_n : base_error_computer<AreCollidingPredicate>
	{
	public:

		explicit error_computer_O_n (const std::vector<li>& initial_genome) : n(initial_genome.size())
		{
			current_genome = initial_genome;
			recompute_error();
		}

		void experience_mutation (size_t figure_index, li new_position)
		{
			li old_position = current_genome[figure_index];

			for (li old_fig_index = 0; old_fig_index < n; ++old_fig_index) {
				auto old_figure_coord = std::pair { old_fig_index, current_genome[old_fig_index] };

				// Subtract old ones:
				if (AreCollidingPredicate::compute(
						{ figure_index, old_position }, old_figure_coord
				)) {
					current_beating_amount--;
				}

				if (AreCollidingPredicate::compute(
						{ figure_index, new_position }, old_figure_coord
				)) {
					current_beating_amount++;
				}
			}

			current_genome[figure_index] = new_position;
		}

		[[nodiscard]] li get_beating_amount () const
		{
			return current_beating_amount;
		}

	private:

		void recompute_error ()
		{
			current_beating_amount = base_chess_error<AreCollidingPredicate>::compute(
				enumerate_chess_figures(current_genome)
			);
		}

		const li n = 0;
		std::vector<li> current_genome;


		// std::vector<std::vector<li>> beating_list;
		li current_beating_amount = 0;
	};

	template <class AreCollidingPredicate>
	class error_computer_O_1 : base_error_computer<AreCollidingPredicate>
	{
	public:
		explicit error_computer_O_1 (const std::vector<li>& initial_genome) : n(initial_genome.size())
		{
			current_genome = initial_genome;
			recompute_error();
		}

	private:

		void recompute_error ()
		{
			current_beating_amount = base_chess_error<AreCollidingPredicate>::compute(
					enumerate_chess_figures(current_genome)
			);
		}

		const li n = 0;
		std::vector<li> current_genome;

		li current_beating_amount = 0;

		// TODO: make it work as ≈ O(1): maintain lists for all the diagonals, all the rows and colons
		// (4 total)
		std::vector<std::vector<li>> for_diagonals;

	}

}
