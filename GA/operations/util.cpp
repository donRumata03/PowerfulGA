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

	population materialize_population (const light_population &l_pop)
	{
		population res;
		res.reserve(l_pop.size());

		for (auto p : l_pop) {
			res.emplace_back(*p);
		}

		return res;
	}

	void renew_random()
	{
		std::random_device rd {};
		// std::mt19937 mt { rd() };
//		srand(mt());
		srand(rd());

		// for (int i = 0; i < 100; i++) rand();
	}



	genome_quantities calculate_genome_quantities (size_t population_size, const genome_quantity_orientation &orient_params)
	{
		// TODO: add possibility for client to choose how much do the hazing vary through time

		/// New population is:
		///
		/// 	best_genome
		///			+
		/// 	hyper_elite
		///			+
		/// 	usual_elite
		///			+
		///		 children
		///			=
		///    POPULATION_SIZE

		/// Also modifies fit pows according to the algorithm_progress_percent -> progress_coefficient

		// //////////////////////////////////////////////////////////////////////////////////////////////////////////////

		assert(orient_params.hazing_percent < 1 && orient_params.hazing_percent > 0);
		assert(population_size >= 3);

		// double progress_coefficient = square(orient_params.algorithm_progress_percent) * 3; /// Multiplying by 3 to save the average value
		/// 	because the Integral of x^2 from 0 to 1 is ⅓

		double progress_coefficient = std::min(1.2 * orient_params.algorithm_progress_percent, 1.) * (12./7); /// Multiplying by 12./7 to save the average value
		/// 	because the Integral of `min(1.2 * x, 1.)` from 0 to 1 is 7/12
		//              so, progress_coefficient varies from `0` to `12 / 7`


		// double new_hazing_percent = orient_params.hazing_percent;
		// double new_hazing_percent = cut(orient_params.hazing_percent * 2 * orient_params.algorithm_progress_percent, 0.01, 0.9);
		double new_hazing_percent = cut(orient_params.hazing_percent * progress_coefficient, 0.05, 0.9);

		// std::cout << console_colors::bold << console_colors::yellow << "Current Hazing: " << new_hazing_percent << console_colors::remove_all_colors << std::endl;

		auto next_eliting_step = [](size_t space_left, double current_hazing) -> size_t {
			return size_t(std::clamp(std::round(current_hazing * space_left), 0., double(space_left)));
		};

		auto get_advanced_hazing = [](double some_hazing) { return sqrt(some_hazing); };
		auto get_slightly_advanced_hazing = [](double some_hazing) { return pow(some_hazing, 0.7); };

		auto get_diminished_hazing = [](double some_hazing) { return square(some_hazing); };
		auto get_slightly_diminished_hazing = [](double some_hazing) { return pow(some_hazing, 1.11); };

		auto slightly_move_to_half = [](double some_hazing){ return sqrt(0.5 * some_hazing); };

		// Split parents and privileged:
		size_t all_elite_number = next_eliting_step(population_size, get_slightly_diminished_hazing(new_hazing_percent)); // get_diminished_hazing is to add some data
		size_t child_number = population_size - all_elite_number;
		if (child_number % 2) // Make it dividable by two: <- why??? IT ISN`T NECESSARY!!!
		{
			if (child_number <= population_size - 1) {
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

		size_t parent_number = child_number * 2;

		// Separate usual elite and higher elite sorts:
		size_t non_usual_elite_number = next_eliting_step(all_elite_number, new_hazing_percent);
		size_t usual_elite_number = all_elite_number - non_usual_elite_number;

		// Separate best genome and hyper elite:
		size_t hyper_elite_number, best_genome_number;
		if (orient_params.use_best_genome) {
			best_genome_number = next_eliting_step(non_usual_elite_number, slightly_move_to_half(slightly_move_to_half(new_hazing_percent)));
			hyper_elite_number = non_usual_elite_number - best_genome_number;
		}
		else {
			hyper_elite_number = non_usual_elite_number;
			best_genome_number = 0;
		}

		assert(child_number + usual_elite_number + hyper_elite_number + best_genome_number == population_size);


#ifndef NDEBUG
		size_t min_suspicious_number = std::numeric_limits<size_t>::max() - 10'000'000; // IT`S VERY BIG!!
		assert(child_number <= min_suspicious_number);
		assert(usual_elite_number <= min_suspicious_number);
		assert(hyper_elite_number <= min_suspicious_number);
		assert(best_genome_number <= min_suspicious_number);
#endif

		/// TODO: modify the fit pows here:

		return genome_quantities \
		{
				.population_size = population_size,
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
		os << "{\n\tparent_fit_pow: " << quantities.parent_fit_pow << " \n\tparent_number: " << quantities.parent_number
		   << " \n\tchild_number: " << quantities.child_number << " \n\tusual_elite_fit_pow: " << quantities.usual_elite_fit_pow
		   << " \n\tusual_elite_number: " << quantities.usual_elite_number << " \n\thyper_elite_fit_pow: "
		   << quantities.hyper_elite_fit_pow << " \n\thyper_elite_number: " << quantities.hyper_elite_number
		   << " \n\tbest_genome_number: " << quantities.best_genome_number << "\n}";
		return os;
	}

	void view_quantity_dynamic (size_t population_size, genome_quantity_orientation orient_params)
	{
		size_t dynamic_iterations = 10'000;

		std::vector<double> child_dynamic(dynamic_iterations),
							usual_elite_dynamic(dynamic_iterations),
							hyper_elite_dynamic(dynamic_iterations),
							best_genome_dynamic(dynamic_iterations);

		for (size_t iteration = 0; iteration < dynamic_iterations; ++iteration) {
			double completion_percent = double(iteration) / dynamic_iterations;
			orient_params.algorithm_progress_percent = completion_percent;

			auto this_quantity = calculate_genome_quantities(population_size, orient_params);

			child_dynamic[iteration]       = this_quantity.child_number;
			usual_elite_dynamic[iteration] = this_quantity.usual_elite_number;
			hyper_elite_dynamic[iteration] = this_quantity.hyper_elite_number;
			best_genome_dynamic[iteration] = this_quantity.best_genome_number;
		}

		add_to_plot(enumerate<double>(child_dynamic),       { .name = "Children" });
		add_to_plot(enumerate<double>(usual_elite_dynamic), { .name = "Usual Elite" });
		add_to_plot(enumerate<double>(hyper_elite_dynamic), { .name = "Hyper Elite" });
		add_to_plot(enumerate<double>(best_genome_dynamic), { .name = "Best Genome" });


		show_plot({ .window_title = "Hazing percent = "s + std::to_string(orient_params.hazing_percent) });
	}

}
