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
inline void display_chess_positioning(const std::vector<std::vector<bool>>& figure_matrix, const std::string& figure_type = /*"Queen"*/"♛") {
	// auto matrix = figure_set_to_matrix(figure_positions);
	size_t n = figure_matrix.size();

	ft_set_u8strwid_func([](const void *beg, const void *end, size_t *width) -> int {
			const char *emojis[] = { "♛", "♞" }; // , "❌"
			// const size_t sz = sizeof(emojis) / sizeof(emojis[0]);
			const size_t raw_len = (const char *) end - (const char *) beg;

			for (auto & emoji : emojis) {
				if (memcmp(beg, emoji, std::min(strlen(emoji), raw_len)) == 0) {
					*width = 2; /* On my terminal emojis have width of 2 chars */
					return 0;
				}
			}
			return 1;
		});

	auto formatted_table = fort::utf8_table{};
	formatted_table.set_border_style(FT_DOUBLE_STYLE);
	formatted_table << fort::header;

	for (size_t i = 0; i < n; ++i) {
		formatted_table.column(i).set_cell_text_align(fort::text_align::center);
		// formatted_table.column(i).set_cell_min_width(5);
		formatted_table.row(i).set_cell_row_type(fort::row_type::common);
		for (size_t j = 0; j < n; ++j) {
			// if (j) std::cout << "|";
			formatted_table << (figure_matrix[i][j] ? figure_type : /*"Nothing"*/" "/*"❌"*/);
		}
		formatted_table << fort::endr << fort::separator;
	}
	std::cout << formatted_table.to_string() << std::endl;
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

