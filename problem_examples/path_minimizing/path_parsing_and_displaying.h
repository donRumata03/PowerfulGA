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
