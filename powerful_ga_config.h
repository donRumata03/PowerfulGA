//
// Created by vova on 25.12.2020.
//


#pragma once
#include <pythonic.h>

constexpr const char* powerful_ga_config_file_path = __FILE__;
inline std::filesystem::path powerful_ga_base_dir =
		get_path_folder(powerful_ga_config_file_path);


