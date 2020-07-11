#include "ga_util.h"

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
	population generate_population(const std::vector<std::pair<double, double>>& ranges, const size_t amount)
	{
		size_t genome_size = ranges.size();
		population res(amount);

		std::vector<std::vector<double>> genome_randoms(genome_size);
		for (int i = 0; i < genome_size; i++) {
			genome_randoms[i] = uniform_distribute(ranges[i].first, ranges[i].second, amount);
		}


		for (size_t i = 0; i < amount; i++) {
			res[i].reserve(genome_size);
			for (size_t j = 0; j < genome_size; j++) {
				res[i].emplace_back(genome_randoms[j][i]);
			}
		}

		return res;
	}

	void mutate(genome& target_genome, const std::vector<double>& sigmas, const double target_gene_number, const normalizer& normaaa)
	{
		auto real_gene_number = static_cast<size_t>(cut(std::round(normaaa.generate(target_gene_number, target_gene_number / 2)), 0, target_genome.size()));
		std::vector<size_t> mutate_indexes = uniform_int_distribute(0, target_genome.size() - 1, real_gene_number);
		// vector<double> mutate_values = normal_distribute(0, sigma, real_gene_number);
		// cout << mutate_indexes << " " << mutate_values << endl;
		for (size_t index = 0; index < mutate_indexes.size(); index++) {
			double this_mutate_value = normaaa.generate(0, sigmas[index]);
			target_genome[mutate_indexes[index]] += this_mutate_value;
		}
	}


	size_t get_matting_index(std::vector<double>& russian_roulette, const double value)
	{
		size_t l = -1, r = russian_roulette.size();

		while (l != r - 1) {
			size_t mid = (r + l) / 2;
			if (russian_roulette[mid] > value) {
				r = mid;
			}
			else {
				l = mid;
			}
		}

		return r;
	}


	population select_matting_pool(const population& genomes, const std::vector<double>& fitnesses, const size_t amount, const double fit_dependence)
	{
		size_t size = genomes.size();
		std::vector<double> russian_roulette(size);

		double fit_sum = 0;

		for (double val : fitnesses) fit_sum += pow(val, fit_dependence);


		bool is_first = true;
		size_t index = 0;
		for (auto& fit : fitnesses) {
			// double this_prob = (!sqr_dependence) ? (fit / fit_sum) : square(fit) / fit_sum; // TODO: make square dependence of probability and fitness!
			double this_prob = pow(fit, fit_dependence) / fit_sum;
			russian_roulette[index] = (is_first ? this_prob : this_prob + russian_roulette[index - 1]);
			is_first = false;
			index++;
		}


		std::vector<std::vector<double>> res(amount);
		auto randoms = uniform_distribute(0, 1, amount);
		for (int i = 0; i < amount; i++) {
			auto this_value = randoms[i];
			size_t this_index = get_matting_index(russian_roulette, this_value);
			res[i] = genomes[this_index];
		}

		return res;

	}

	light_population select_matting_pool(const light_population& genomes, const std::vector<double>& fitnesses, const size_t amount, const double fit_dependence)
	{
		size_t size = genomes.size();
		std::vector<double> russian_roulette(size);

		double fit_sum = 0;

		for (double val : fitnesses) fit_sum += pow(val, fit_dependence);


		bool is_first = true;
		size_t index = 0;
		for (auto& fit : fitnesses) {
			// double this_prob = (!sqr_dependence) ? (fit / fit_sum) : square(fit) / fit_sum; // TODO: make square dependence of probability and fitness!
			double this_prob = pow(fit, fit_dependence) / fit_sum;
			russian_roulette[index] = (is_first ? this_prob : this_prob + russian_roulette[index - 1]);
			is_first = false;
			index++;
		}


		light_population res(amount);
		auto randoms = uniform_distribute(0, 1, amount);
		for (size_t i = 0; i < amount; i++) {
			auto this_value = randoms[i];
			size_t this_index = get_matting_index(russian_roulette, this_value);
			res[i] = genomes[this_index];
		}

		return res;
	}


	population perform_crossover_matting(parents_t & parents) {
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

	light_parents_t distribute_pairs(light_population& pop, const size_t pair_amount, const bool allow_gay_marriage)
	{
		light_parents_t result;
		result.reserve(pair_amount);

		size_t pop_size = pop.size();
		
		for(size_t p_index = 0; p_index < pair_amount; p_index++)
		{
			size_t father_index = randint(0, pop_size);
			size_t mother_index = randint(0, pop_size);
			if (!allow_gay_marriage)
				while (mother_index == father_index) mother_index = randint(0, pop_size);
			
			result.emplace_back(pop[mother_index], pop[father_index]);
		}
		return result;
	}

	std::pair<double, genome> find_best_genome(population& pop, const std::vector<double>& fits)
	{
		double best_fitness = fits[0];
		genome* g = pop.data();
		for (size_t index = 0; index < pop.size(); index++) if (best_fitness < fits[index])
		{
			best_fitness = fits[index];
			g = &pop[index];
		}

		return { best_fitness, *g };
	}

	std::pair<double, genome> find_best_genome(light_population& pop, const std::vector<double>& fits)
	{
		double best_fitness = fits[0];
		genome* g = pop[0];
		for (size_t index = 0; index < pop.size(); index++) if (best_fitness < fits[index])
		{
			best_fitness = fits[index];
			g = pop[index];
		}

		return { best_fitness, *g };
	}
	
	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
		const double hyper_elite_fit_pow, const double usual_elite_fit_pow, const double parent_fit_pow, 
		const size_t usual_elite_number, const size_t hyper_elite_number, const size_t best_genome_number, const genome& best_genome,
		const matting_mode mode)
	{
		light_population souls;
		
		souls.reserve(pop.size());
		for (genome& val : pop) souls.push_back(&val);

		light_population parents = select_matting_pool(souls, fitnesses, pop.size(), parent_fit_pow);
		light_population usual_elite = select_matting_pool(souls, fitnesses, usual_elite_number, usual_elite_fit_pow);
		light_population hyper_elite = select_matting_pool(souls, fitnesses, hyper_elite_number, hyper_elite_fit_pow);

		
		size_t child_number = pop.size() - usual_elite_number - hyper_elite_number - best_genome_number;

		
		// cout << "Parents: " << endl;
		// for (auto parent : parents) cout << *parent << endl;

		// cout << "Elite: " << endl;
		// for (auto& p : elite) cout << *p << endl;

		
		light_parents_t formed_pairs = distribute_pairs(parents, child_number);

		// cout << "Best: " << 1 / find_best_genome(pop, fitnesses).first << " " << find_best_genome(pop, fitnesses).second << endl;
		
		// cout << endl << "Pairs: " << endl;
		// for (auto& p : formed_pairs) cout << *p.first << " " << *p.second << endl;

		population res = perform_crossover(formed_pairs, normaaaaa, mode);
		
		res.reserve(pop.size());

		// cout << res << endl;
		
		for (auto& elite_person : usual_elite) res.emplace_back(*elite_person);
		for (auto& elite_person : hyper_elite) res.emplace_back(*elite_person);

		for (size_t index = 0; index < best_genome_number; index++) res.emplace_back(best_genome);
		
		return res;
	}


	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, const matting_mode mode)
	{
		assert(parents.second->size() == parents.first->size());
		size_t genome_size = parents.first->size();
		genome res(genome_size);

		for (size_t gene_index = 0; gene_index < genome_size; gene_index++)
		{
			double this_gene = 0;

			double left_gene = parents.first->operator[](gene_index);
			double right_gene = parents.second->operator[](gene_index);

			double gene_mu = (left_gene + right_gene) / 2;
			double variance = std::max(left_gene, right_gene) - gene_mu;

			double new_variance;
			
			switch(mode)
			{
			case matting_mode::uniform:
				this_gene = randval(gene_mu - variance * 1.5, gene_mu + variance * 1.5);
				break;
			case matting_mode::low_variance_normal:
				new_variance = variance / 2;
			case matting_mode::middle_variance_normal:
				new_variance = variance;
			case matting_mode::high_variance_normal:
				new_variance = variance * 1.5;
			case matting_mode::random_variance_normal:
				new_variance = exp(randval( log(variance / 2.), log(variance * 1.5) ));
				this_gene = normaaaaa.generate(gene_mu, new_variance); // normal_distribute(gene_mu, new_variance, 1)[0];
				break;
				// TODO: use fast normal number generator!
			case matting_mode::high_variance_genetic:
			case matting_mode::low_variance_genetic:
				bool high_mode = mode == matting_mode::high_variance_genetic;
				double this_mu = 0;
				double this_sigma = 0;

				if (!high_mode) {
					size_t mode_mode_val = randint(0, 5);

					switch (mode_mode_val)
					{
					case 0:
					case 1:
						this_mu = left_gene;
						this_sigma = variance * 0.5;
						break;
					case 2:
						this_mu = gene_mu;
						this_sigma = variance * 0.2;
						break;
					case 3:
					case 4:
						this_mu = right_gene;
						
						this_sigma = variance * 0.5;
						break;
					default:
						std::cout << "Error with randint!!!!" << std::endl;
					}
				}
				else
				{
					size_t mode_mode_val = randint(0, 9);

					switch (mode_mode_val)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						this_mu = left_gene;
						this_sigma = variance * 0.8;
						break;
					case 4:
						this_mu = gene_mu;
						this_sigma = variance * 0.1;
						break;
					case 5:
					case 6:
					case 7:
					case 8:
						this_mu = right_gene;

						this_sigma = variance * 0.8;
						break;
					default:
						std::cout << "Error with randint!!!!" << std::endl;
					}
				}
				
				this_gene = normaaaaa.generate(this_mu, this_sigma); // normal_distribute(this_mu, this_sigma, 1)[0];
			}
			
			res[gene_index] = this_gene;
		}

		return res;
	}

	population perform_crossover(parents_t& parents, const normalizer& normaaaaa, const matting_mode mode) {
		population res;
		res.reserve(parents.size() * 2);
		for (auto& p : parents) {
			assert(p.second.size() == p.first.size());

			res.push_back(mat_parents({ &p.first, &p.second }, normaaaaa, mode));
			
			/*
			size_t crossover_point = randint(size_t(0), p.first.size());
			// cout << "Crossover point: " << crossover_point << endl;
			genome& child1 = res.emplace_back();
			genome& child2 = res.emplace_back();
			child1 = p.first;
			child2 = p.first;

			copy(p.first.begin(), p.first.begin() + crossover_point, child1.begin());
			copy(p.second.begin() + crossover_point, p.second.end(), child1.begin() + crossover_point);

			// cout << "Traveled first! " << child1.size() << " " << child2.size() << endl;
			copy(p.second.begin(), p.second.begin() + crossover_point, child2.begin());
			copy(p.first.begin() + crossover_point, p.first.end(), child2.begin() + crossover_point);
			*/
		}
		return res;
	}


	population perform_crossover(light_parents_t& parents, const normalizer& normaaaaa, const matting_mode mode) {
		population res;
		res.reserve(parents.size() * 2);
		for (auto& p : parents) {
			assert(p.second->size() == p.first->size());

			res.push_back(mat_parents({ p.first, p.second }, normaaaaa, mode));
		}
		return res;
	}
	
}


void renew_random()
{
	srand(unsigned(time(nullptr)));
	for (int i = 0; i < 100; i++) rand();
}

