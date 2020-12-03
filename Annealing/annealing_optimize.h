//
// Created by Vova on 03.12.2020.
//

#pragma once

#include <pythonic.h>
#include "default_annealing_operations.h"


struct AnnealingOptimizeParameters
{
	/// Finishing conditions:
	size_t iterations = 0;
	double exiting_value = -std::numeric_limits<double>::infinity();

	///
	double typical_temperature = 1;

	size_t genes_in_genome = 0;
};



/**
 *
 * @tparam GenomeElement: Function always operates on std::vector<T>, where T is GenomeElement
 * @param energy_functor: should return a quantitative characteristics od
 * @param mutation_functor: is responsible of being called with «const Genome&, double» and should return new Genome,
 * 												mutated as much as its second argument says,
 * 													it's being smoothly changed from 1 to 0
 * @param temperature_changing_functor: Outputs Temperature at particular time point (from 0 to 1) and is expected to output values from 0 to 1
 * @return
 */
template<class GenomeElement, class EnergyFunctor, class GenomeGenerationFunctor,
		class MutationFunctor, class TemperatureChangingFunctor>
std::pair<std::vector<GenomeElement>, double> annealing_optimize(
		const EnergyFunctor& energy_functor,
		const AnnealingOptimizeParameters& params,

		const GenomeGenerationFunctor& genome_generation_functor,
		const MutationFunctor& mutation_functor,
		const TemperatureChangingFunctor& temperature_changing_functor = exp_temperature_dynamic)
{

	using Genome = std::vector<GenomeElement>;

	static_assert(std::is_invocable_v<EnergyFunctor, Genome>, "False EnergyFunctor's input parameters");
	static_assert(std::is_same_v<decltype(energy_functor(Genome())), double>, "False EnergyFunctor's return type");

	static_assert(std::is_invocable_v<MutationFunctor, const Genome&, double>, "False MutationFunctor's input parameters");
	static_assert(std::is_same_v<decltype(mutation_functor(Genome(), double{})), Genome>, "False MutationFunctor's return type");

	static_assert(std::is_invocable_v<TemperatureChangingFunctor, double>, "False TemperatureChangingFunctor's input parameters");
	static_assert(std::is_same_v<decltype(temperature_changing_functor(double{})), double>, "False TemperatureChangingFunctor's return type");

	static_assert(std::is_invocable_v<GenomeGenerationFunctor, size_t>, "False GenomeGenerationFunctor's input parameters");
	static_assert(std::is_same_v<decltype(genome_generation_functor(size_t())), Genome>, "False GenomeGenerationFunctor's return type");



	Genome p = genome_generation_functor(params.genes_in_genome);
	double last_energy = energy_functor(p);

	Genome best_genome = p;
	double best_energy = last_energy;


	for (size_t iteration = 0; iteration < params.iterations; ++iteration) {
		double completion_percent = double(iteration) / params.iterations;
		double temperature = params.typical_temperature * temperature_changing_functor(completion_percent);

		std::cout <<
			"Iteration " << iteration <<
			" : Completion Percent: " << completion_percent <<
			", temperature: " << temperature << ", Current Genome: " << p;

		Genome mutated = mutation_functor(p, 1 - completion_percent);

		std::cout << ", Mutated genome: " << mutated;

		double this_energy = energy_functor(mutated);
		double dE = this_energy - last_energy;

		if (dE <= 0 or std::exp(-dE / temperature) > pythonic_random()) {
			std::cout << "Mutated Genome accepted!";

			p = mutated;
			last_energy = this_energy;

			if (this_energy < best_energy) {
				best_genome = mutated;
				best_energy = this_energy;
			}

			if (this_energy <= params.exiting_value) {
				break;
			}
		}

		std::cout << std::endl;
	}

	return { best_genome, best_energy };
}


