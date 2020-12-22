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


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////					Basic Error computing 									////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

		void experience_mutation (size_t figure_index, li new_position) override
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

		[[nodiscard]] li get_beating_amount () const override
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

	};

//	inline void mutate_one (std::vector<li>& positions, size_t steps)
//	{
//
//	}

	// class chess1d_permutator;

	class chess1d_permutator
	{
	public:
		struct permutation_info {
			size_t figure_index {};
			li new_position {};
		};
		using mutation_descriptor = std::vector<permutation_info>;

		enum class permutation_type {
			movement,
			swapping
		};
		static constexpr permutation_type current_permutation_type = permutation_type::swapping;

		// TODO: allow swapping mutation
	private:

		double permute_intensiveness_factor = 0;
		mutable mutation_descriptor last_mutation_descriptor;

	public:
		explicit chess1d_permutator(double _permute_intensiveness_factor)
				: permute_intensiveness_factor(_permute_intensiveness_factor)
		{}

		std::vector<li> operator() (const std::vector<li>& old_genome, double amount) const {
			last_mutation_descriptor.clear();

			std::vector<li> res = old_genome;
			li n = old_genome.size();

			if constexpr (current_permutation_type == permutation_type::movement) {
				double target_figure_number = amount * permute_intensiveness_factor;
				double target_movement_distance = amount * permute_intensiveness_factor;

				auto figure_number = size_t(std::round(target_figure_number));
				// std::cout << "Mutating " << figure_number << " figures (" << "target_movement_distance is " << target_movement_distance << "): " << std::endl;

				for (size_t mutation_index = 0; mutation_index < figure_number; ++mutation_index) {
					auto gen = std::mt19937 { std::random_device {}() };
					auto mutating_figure_index = size_t(randint(0LL, n, gen));

					double generated_distance = normal_distribute(0, target_movement_distance * 1.5, 1)[0];
					auto pos_change = li(std::round(generated_distance));

					li new_pos = std::clamp(li
							                        (res[mutating_figure_index]) + pos_change, 0LL, n - 1);

					// std::cout << "\t moving figure №" << mutating_figure_index
					//           << " from " << res[mutating_figure_index]
					//           << " to " << new_pos
					//           << std::endl;

					res[mutating_figure_index] = new_pos;

					last_mutation_descriptor.push_back(
							{ .figure_index = mutating_figure_index, .new_position = new_pos });
				}
			}
			else {
				// Swapping mutation:
				double target_figure_number = permute_intensiveness_factor * amount;
				double target_pairs = target_figure_number / 2;
				double generated_pair_number = normal_distribute(target_pairs, target_pairs / 3, 1)[0];
				auto pairs_to_swap = size_t(std::clamp(generated_pair_number, 0., double(n - 1)));

				std::mt19937 gen{ std::random_device{}() };
				for (size_t mutation_index = 0; mutation_index < pairs_to_swap; ++mutation_index) {

					size_t first = randint(0ULL, size_t(n), gen);
					size_t second = randint(0ULL, size_t(n), gen);

					last_mutation_descriptor.push_back({ .figure_index = first, .new_position = res[second] });
					last_mutation_descriptor.push_back({ .figure_index = second, .new_position = res[first] });

					std::swap(res[first], res[second]);
				}
			}

			return res;
		}

		[[nodiscard]] mutation_descriptor get_last_mutation_descriptor() const {
			return last_mutation_descriptor;
		}
	};

	class final_error_computer
	{
	public:
		double operator() (const std::vector<li>& genome,
					const chess1d_permutator::mutation_descriptor* info = nullptr, std::optional<double> previous_error = std::nullopt) const {

			if (info and previous_error) {
				// O(n) recounting:
				// (If only 3 arguments are provided, the genome is «old genome»)

				auto computer = error_computer_O_n<are_queens_colliding>(genome);
				for(auto& mutation : *info) {
					computer.experience_mutation(mutation.figure_index, mutation.new_position);
				}

				return computer.get_beating_amount();
			}
			else if (info or previous_error) {
				throw std::runtime_error("Can't compute error: no info or no previous_error");
			}

			// Simple O(n^2) recounting:
			// If only one argument is provided, it's «new genome»
			return base_chess_error<are_queens_colliding>::compute(
					enumerate_chess_figures(genome)
			);
		}

//		void be_informed_about_change(size_t figure_index, li new_position) {
//			computer->experience_mutation(figure_index, new_position);
//		}

	private:
		// mutable std::optional<error_computer_O_n<are_queens_colliding>> computer = std::nullopt;
	};

}
