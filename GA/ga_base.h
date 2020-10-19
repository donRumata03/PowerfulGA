#pragma once
#include <pythonic.h>



namespace GA {
	// Some typedefs:

	using Genome = std::vector<double>;

	using Population = std::vector<Genome>;
	using LightPopulation = std::vector<Genome*>;
	
	using Parents = std::vector<std::pair<Genome, Genome>>;
	using LightParents = std::vector<std::pair<Genome*, Genome*>>;
}
