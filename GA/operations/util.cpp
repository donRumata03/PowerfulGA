//
// Created by Vova on 18.09.2020.
//

#include "util.h"

namespace GA
{
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

	void renew_random()
	{
		srand(unsigned(time(nullptr)));
		for (int i = 0; i < 100; i++) rand();
	}



	genome_quantities calculate_genome_quantities (size_t population_size, const genome_quantity_orientation &orient_params)
	{
		/// New population is:
		///
		/// 	best_genome (given number of times)
		///			+
		/// 	hyper_elite
		///			+
		/// 	usual_elite
		///			+
		///		 children
		///			=
		///    POPULATION_SIZE

		// //////////////////////////////////////////////////////////////////////////////////////////////////////////////

		assert(orient_params.hazing_percent < 1 && orient_params.hazing_percent > 0);
		assert(population_size >= 5);

		double progress_coefficient = square(orient_params.algorithm_progress_percent) * 3; /// Multiplying by 3 to save the average value
		/// 	because the Integral of x^2 from 0 to 1 is ¹/‰↑∞←³

		// double new_hazing_percent = orient_params.hazing_percent;
		// double new_hazing_percent = cut(orient_params.hazing_percent * 2 * orient_params.algorithm_progress_percent, 0.01, 0.9);
		double new_hazing_percent = cut(orient_params.hazing_percent * progress_coefficient, 0.01, 0.9);


		/// Simple implementation without hazing increasing by hazing percent:

		auto next_eliting_step = [](size_t space_left, double current_hazing) -> size_t {
			return size_t(cut(std::round(current_hazing * space_left), 0, space_left));
		};

		auto get_advanced_hazing = [](double some_hazing) { return sqrt(some_hazing); };
		auto get_slightly_advanced_hazing = [](double some_hazing) { return sqrt(some_hazing); };

		auto get_diminished_hazing = [](double some_hazing) { return square(some_hazing); };

		// Split parents and privileged:
		size_t all_elite_number = next_eliting_step(population_size, get_diminished_hazing(new_hazing_percent)); // get_diminished_hazing is to add some data
		size_t child_number = population_size - all_elite_number;
		if (child_number % 2) // Make it dividable by two:
		{
			if (child_number != population_size) {
				// Increase children number and decrease elite number if we can
				child_number++;
				all_elite_number--;
			}
			else {
				// Otherwise: Increase elite number and decrease children number
				child_number--;
				all_elite_number++;
			}
		}

		size_t parent_number = child_number / 2;

		// Separate usual elite and higher elite sorts:
		size_t non_usual_elite_number = next_eliting_step(all_elite_number, new_hazing_percent);
		size_t usual_elite_number = all_elite_number - non_usual_elite_number;

		// Separate best genome and hyper elite:
		size_t hyper_elite_number = next_eliting_step(all_elite_number, new_hazing_percent);
		size_t best_genome_number = non_usual_elite_number;

		return genome_quantities \
		{
				.parent_fit_pow = orient_params.parent_fit_pow,
				.parent_number = parent_number,
				.child_number = child_number,
				.usual_elite_fit_pow = orient_params.usual_elite_fit_pow,
				.usual_elite_number = usual_elite_number,
				.hyper_elite_fit_pow = orient_params.hyper_elite_fit_pow,
				.hyper_elite_number = hyper_elite_number,
				.best_genome_number = best_genome_number,
		};
	}

	std::ostream &operator<< (std::ostream &os, const genome_quantities &quantities)
	{
		os << "{\nparent_fit_pow: " << quantities.parent_fit_pow << " \nparent_number: " << quantities.parent_number
		   << " \nchild_number: " << quantities.child_number << " \nusual_elite_fit_pow: " << quantities.usual_elite_fit_pow
		   << " \nusual_elite_number: " << quantities.usual_elite_number << " \nhyper_elite_fit_pow: "
		   << quantities.hyper_elite_fit_pow << " \nhyper_elite_number: " << quantities.hyper_elite_number
		   << " \nbest_genome_number: " << quantities.best_genome_number << "}";
		return os;
	}
}
