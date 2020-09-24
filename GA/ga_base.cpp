#include <GA/operations/crossover.h>
#include "ga_base.h"

/*
template<class distributor, class ... Args>
vector<double> distribute(Args... args, size_t samples)
{
	auto d = distributor(args...);
	std::random_device rd{};
	std::mt19937 gen{ rd() };

	vector <double> res;
	for (int i = 0; i < samples; i++) {
		res.push_back(d(gen));
	}

	return res;
}


vector<double> normal_distribute(const double mu, const double sigma, const size_t samples)
{
	return distribute < std::normal_distribution <double>, double, double >(mu, sigma, samples);
}


vector<double> uniform_distribute(const double from, const double to, const size_t samples)
{
	return distribute < std::uniform_real_distribution<double>, double, double >(from, to, samples);

}

*/

namespace GA
{


	/**
	 * 			Dummy crossover function, DEPRECATED!
	 */
	population perform_dummy_crossover_matting(parents_t & parents) {
// #pragma message("Deprecated function used!")
		population res;
		res.reserve(parents.size() * 2);
		for (auto& p : parents) {
			assert(p.second.size() == p.first.size());
			size_t crossover_point = randint(size_t(0), p.first.size());
			// cout << "Crossover point: " << crossover_point << endl;
			genome & child1 = res.emplace_back();
			genome & child2 = res.emplace_back();
			child1 = p.first;
			child2 = p.first;

			copy(p.first.begin(), p.first.begin() + crossover_point, child1.begin());
			copy(p.second.begin() + crossover_point, p.second.end(), child1.begin() + crossover_point);

			// cout << "Traveled first! " << child1.size() << " " << child2.size() << endl;
			copy(p.second.begin(), p.second.begin() + crossover_point, child2.begin());
			copy(p.first.begin() + crossover_point, p.first.end(), child2.begin() + crossover_point);
		}
		return res;
	}



}


