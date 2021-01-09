//
// Created by Vova on 23.12.2020.
//

#pragma once


#include <pythonic.h>

#include <powerful_ga_config.h>

inline std::filesystem::path path_minimizing_data_path = \
		powerful_ga_base_dir / "problem_examples" / "path_minimizing" / "statement_data.txt";
// R"(D:\Projects\Utilities\GA\problem_examples\path_minimizing\statement_data.txt)";

inline std::vector<std::vector<bool>> parse_way_matrix(const std::string& str) {
	std::vector<std::vector<bool>> res;

	std::optional<size_t> line_length;

	auto splitted_lines = split_lines(str);
	for (auto& l : splitted_lines) {
		auto words = split(l, { ' ' }); // split_words(l);
		for (auto& word : words) {
			word = cut_spaces(word);
			if (is_in('#', word) and is_in('.', word)) {
				// This is the word we're looking for!
				auto& v = res.emplace_back();

				for (auto c : word) {
					if (c == '#') {
						v.push_back(true);
					}
					else if (c == '.') {
						v.push_back(false);
					}
					else {
						throw std::runtime_error("Can't parse");
					}
				}

				if (!line_length) {
					line_length = v.size();
				}
				else {
					if (line_length != v.size()) {
						throw std::runtime_error("Different line lengths!");
					}
				}
			}
		}
	}

	return res;
}

inline std::vector<point> path_matrix_to_points(const std::vector<std::vector<bool>>& matrix) {
	std::vector<point> res;

	size_t y_index = 0;
	for (auto& row : matrix) {
		size_t x_index = 0;
		for(const auto& position : row) {
			if (position) {
				res.emplace_back(double(x_index), double(y_index));
			}
			x_index++;
		}
		y_index++;
	}

	return res;
}

inline std::string path_point_to_code(std::pair<li, li> point) {
	std::string letter;
	li for_letter = point.first;
	if(for_letter < 26) {
		letter = std::string {char('a' + char(for_letter))};
	}
	else{
		letter = std::string {char('A' + char(for_letter - 26))};
	}
	std::string digit = std::to_string(52 - point.second);

	return letter + digit;
}


inline std::string path_points_to_answer(const std::vector<point>& path_points) {
	// std::vector<double> real_ys(path_points.size());
	// std::transform(path_points.begin(), path_points.end(), real_ys.begin(), [](const point& pnt){ return pnt.x; });
	// std::cout << *std::max_element(real_ys.begin(), real_ys.end()) << std::endl;

	std::vector<std::string> res_strings(path_points.size());
	std::transform(path_points.begin(), path_points.end(), res_strings.begin(), [](const point& pnt){
		return path_point_to_code(std::pair{ li(std::round(pnt.x)), li(std::round(pnt.y)) });
	});

	return join(" ", res_strings);
}

