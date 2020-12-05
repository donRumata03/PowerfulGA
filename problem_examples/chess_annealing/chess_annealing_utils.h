//
// Created by Vova on 05.12.2020.
//

#pragma once

#include <pythonic.h>
#include <Annealing/annealing_optimize.h>
#include <lib/libfort/fort.hpp>

using li = long long int;



/**
 * @param figure_type: ♞, ♛, …
 */
inline void display_chess_positioning(const std::vector<std::vector<bool>>& figure_matrix, const std::string& figure_type = "♛") {
	// auto matrix = figure_set_to_matrix(figure_positions);
	size_t n = figure_matrix.size();
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < n; ++j) {
			if (j) std::cout << "|";
			std::cout << (figure_matrix[i][j] ? figure_type : " "/*"❌"*/);
		}
		std::cout << "\n";
	}
}

//////////////////////////////////////////		Converting		////////////////////////////////

inline std::vector<std::vector<bool>> figure_set_to_matrix(const std::vector<std::pair<li, li>>& figure_positions, li n) {
	std::vector<std::vector<bool>> matrix(n);
	for (size_t i = 0; i < n; ++i) {
		matrix[i].assign(n, false);
	}

	for (const auto & figure_position : figure_positions) {
		matrix[figure_position.first][figure_position.second] = true;
	}

	return matrix;
}

inline std::vector<std::pair<li, li>> matrix_to_figure_set(const std::vector<std::vector<bool>>& matrix) {
	std::vector<std::pair<li, li>> res;

	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			if (matrix[i][j]) res.emplace_back( i, j );
		}
	}

	return res;
}

template<class T>
std::vector<std::pair<li, li>> split_into_pairs(const std::vector<T>& values) {
	assert(!(values.size() % 2));

	std::vector<std::pair<li, li>> compressed_values(values.size() / 2);
	for (size_t i = 0; i < compressed_values.size(); ++i) {
		compressed_values[i] = { values[2 * i], values[2 * i + 1] };
	}

	return compressed_values;
}

template<class T>
std::vector<T> unpack_pairs(const std::vector<std::pair<T, T>>& pairs) {
	std::vector<T> res(pairs.size() * 2);

	for (size_t i = 0; i < pairs.size(); ++i) {
//		res[2 * i] = pairs[i].first;
//		res[2 * i + 1] = pairs[i].second;
		std::tie(res[2 * i], res[2 * i + 1]) = pairs[i];
	}

	return res;
}

