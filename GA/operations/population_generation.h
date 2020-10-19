//
// Created by Vova on 17.09.2020.
//

#pragma once

#include "../ga_base.h"

namespace GA
{
	Population generate_population(const std::vector<std::pair<double, double>>& ranges, size_t amount);
}

