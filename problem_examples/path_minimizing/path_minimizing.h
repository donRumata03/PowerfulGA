//
// Created by Vova on 20.12.2020.
//

#pragma once

#include "path_minimizing_utils.h"


std::vector<std::vector<bool>> get_default_matrix();


std::pair<std::vector<point>, double> minimize_path(const std::vector<std::vector<bool>>& way_matrix);

void launch_path_minimizing();

