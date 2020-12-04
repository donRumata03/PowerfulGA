//
// Created by Vova on 04.12.2020.
//

#pragma once

using li = long long int;

void launch_chess_queen_arranging();
std::optional<std::vector<std::pair<li, li>>> arrange_chess_queens();

template<class T>
std::pair<T, T> split_into_pairs(const std::vector<T>& values) {
	assert(!(values.size() % 2));

	std::vector<std::pair<li, li>> compressed_values(values.size() / 2);
	for (size_t i = 0; i < compressed_values.size(); ++i) {
		compressed_values[i] = { values[2 * i], values[2 * i + 1] };
	}

	return compressed_values;
}

/**
 * @param figure_type: ♞, ♛, …
 */
void display_chess_positioning(const std::vector<std::pair<li, li>>& figure_positions, size_t n, const std::string& figure_type = "♛") {
	std::vector<std::vector<bool>> matrix(n);
	for (size_t i = 0; i < n; ++i) {
		matrix[i].assign(n, false);
	}

	for (const auto & figure_position : figure_positions) {
		matrix[figure_position.first][figure_position.second] = true;
	}

	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (j) std::cout << " ";
			std::cout << (matrix[i][j] ? figure_type : " "/*"❌"*/);
		}
		std::cout << "\n";
	}
}

template<class AreBeatingPredicate>
struct estimate_chess_error
{
	static_assert(
			std::is_invocable_v<
			        AreBeatingPredicate::compute,
			        std::pair<li, li>, std::pair<li, li>>,
			"False AreBeatingPredicate's input parameters"
	);
	static_assert(std::is_same_v<decltype(
			AreBeatingPredicate::compute(
					std::pair<li, li>(), std::pair<li, li>())
					), bool>,
					"False AreBeatingPredicate's return type"
	);

	long long n {};
	explicit estimate_chess_error(long long _n) : n(_n) {};

	double compute(const std::vector<std::pair<long long, long long>>& queen_positions) {
		size_t beating_count = 0;
		for (size_t first = 0; first < queen_positions.size(); ++first) {
			for (size_t second = first + 1; second < queen_positions.size(); ++second) {
				if (AreBeatingPredicate::compute(queen_positions[first], queen_positions[second])) {
					beating_count++;
				}
			}
		}
		return beating_count;
	}

	double operator() (const std::vector<li>& queen_positioning_data) {
		return compute(split_into_pairs(queen_positioning_data));
	}
};

struct are_queens_colliding {
	static bool compute(std::pair<li, li> q1, std::pair<li, li> q2) {
		auto [x1, y1] = q1;
		auto [x2, y2] = q2;

		if (x1 == x2 || y1 == y2) {
			return true;
		}
		li dx = x2 - x1;
		li dy = y2 - y1;
		if (std::abs(dx) == std::abs(dy)) {
			return true;
		}

		return false;
	}
};

struct are_horses_colliding {
	static bool compute(std::pair<li, li> h1, std::pair<li, li> h2) {
		auto [x1, y1] = h1;
		auto [x2, y2] = h2;

		li dx = x2 - x2;
		li dy = y1 - y2;

		if (
				(std::abs(dx) == 2 and std::abs(dy) == 3)
									or
				(std::abs(dx) == 3 and std::abs(dy) == 2)
			)
		{
			return true;
		}

		return false;
	}
};

struct are_slonopotams_colliding {
	static bool compute(std::pair<li, li> s1, std::pair<li, li> s2) {
		return are_horses_colliding::compute(s1, s2) || are_queens_colliding::compute(s1, s2);
	}
};

using estimate_queens_chess_error = estimate_chess_error<are_queens_colliding>;
using estimate_horses_chess_error = estimate_chess_error<are_horses_colliding>;
using estimate_slonopotams_chess_error = estimate_chess_error<are_slonopotams_colliding>;

