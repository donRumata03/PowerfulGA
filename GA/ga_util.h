#pragma once
#include <pythonic.h>
#include <random>


namespace GA {

	using genome = std::vector<double>;

	using population = std::vector<genome>;
	using light_population = std::vector<genome*>;
	
	using parents_t = std::vector<std::pair<genome, genome>>;
	using light_parents_t = std::vector<std::pair<genome*, genome*>>;

	
	enum class matting_mode
	{
		uniform,

		high_variance_normal,
		middle_variance_normal,
		low_variance_normal,

		random_variance_normal,

		high_variance_genetic,
		low_variance_genetic
	};
	
	population generate_population(const std::vector<std::pair<double, double>>& ranges, size_t amount);
	void mutate(genome& target_genome, const std::vector<double>& sigmas, double target_gene_number, const normalizer& normaaa);

	population select_matting_pool(const population& genomes, const std::vector<double>& fitnesses, size_t amount, double fit_dependence);
	population perform_crossover_matting(parents_t& parents); // Deprecated
	light_parents_t distribute_pairs(light_population& pop, size_t pair_amount, bool allow_gay_marriage = false);

	genome mat_parents(const std::pair<genome*, genome*>& parents, const normalizer& normaaaaa, matting_mode mode = matting_mode::low_variance_genetic); // For test
	population perform_crossover(parents_t& parents, const normalizer& normaaaaa,  matting_mode mode = matting_mode::low_variance_genetic); // Fancy
	population perform_crossover(light_parents_t& parents, const normalizer& normaaaaa,  matting_mode mode = matting_mode::low_variance_genetic); // Fancy

	population make_new_generation(population& pop, const std::vector<double>& fitnesses, const normalizer& normaaaaa,
		double hyper_elite_fit_pow, double usual_elite_fit_pow, double parent_fit_pow, 
		size_t usual_elite_number, size_t hyper_elite_number, size_t best_genome_number, const genome& best_genome,
		matting_mode mode = matting_mode::low_variance_genetic);

	std::pair<double, genome> find_best_genome(population& pop, const std::vector<double>& fits);
	std::pair<double, genome> find_best_genome(light_population& pop, const std::vector<double>& fits);
}

void renew_random();
