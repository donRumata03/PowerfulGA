#pragma once
#include <pythonic.h>



namespace GA {
	// Some typedefs:

	using genome = std::vector<double>;

	using population = std::vector<genome>;
	using light_population = std::vector<genome*>;
	
	using parents_t = std::vector<std::pair<genome, genome>>;
	using light_parents_t = std::vector<std::pair<genome*, genome*>>;
}
