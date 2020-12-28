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
	std::optional<size_t> resurrect_after_iterations = std::nullopt;
};



/**
 *
 * @tparam GenomeElement Function always operates on std::vector<T>, where T is GenomeElement
 * @param energy_functor should return a quantitative characteristics od
 * @param mutation_functor is responsible of being called with «const Genome&, double» and should return new Genome,
 * 												mutated as much as its second argument says,
 * 													it's being smoothly changed from 1 to 0
 * @param temperature_changing_functor Outputs Temperature at particular time point (from 0 to 1) and is expected to output values from 0 to 1
 * @return
 */
template<class GenomeElement, class temp_MutationDescriptor, class EnergyFunctor, class GenomeGenerationFunctor,
		class MutationFunctor, class TemperatureChangingFunctor>
std::pair<std::vector<GenomeElement>, double> annealing_optimize(
		const EnergyFunctor& energy_functor,
		const AnnealingOptimizeParameters& params,

		const GenomeGenerationFunctor& genome_generation_functor,
		const MutationFunctor& mutation_functor,
		const TemperatureChangingFunctor& temperature_changing_functor = default_exp_temperature_dynamic,

		bool output_debug_information = true,
		std::vector<double>* for_vague_history = nullptr,
		std::vector<double>* for_usual_history = nullptr,
		std::vector<double>* for_best_history = nullptr

				)
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

	constexpr bool requires_recounting_support = not std::is_same_v<temp_MutationDescriptor, void>;
	using MutationDescriptor = typename std::conditional<requires_recounting_support, temp_MutationDescriptor, char8_t>::type; // char8_t is just a small type…
	constexpr bool recounting_is_available =
			requires_recounting_support and
			std::is_invocable_v<EnergyFunctor, const Genome&, const MutationDescriptor*, std::optional<double>>;


	if constexpr (requires_recounting_support and not recounting_is_available) {
		static_assert(recounting_is_available, "If descriptor type is set to not void, recounting should be available");
	}
	if constexpr (requires_recounting_support) {
		static_assert(std::is_same_v<decltype(mutation_functor.get_last_mutation_descriptor()), temp_MutationDescriptor>);
	}



	Genome p = genome_generation_functor(params.genes_in_genome);
	double last_energy = energy_functor(p);

	Genome best_genome = p;
	double best_energy = last_energy;

	MutationDescriptor last_mutation_descriptor;
	// Genome temp_genome;

	size_t accepted_gene_count = 0;
	bool has_finished_ahead_of_schedule = false;
	size_t iterations_if_finished_early = 0;
	auto iteration_of_last_improvement = size_t(-1);
	for (size_t iteration = 0; iteration < params.iterations; ++iteration) {
		double completion_percent = double(iteration) / params.iterations;
		double temperature = params.typical_temperature * temperature_changing_functor(completion_percent);

//          std::cout <<
//			"Iteration " << iteration <<
//			" : Completion Percent: " << completion_percent <<
//			", temperature: " << temperature << ", Current Genome: " << p;

		Genome mutated = mutation_functor(p, 1 - completion_percent);
		if constexpr (requires_recounting_support) {
			last_mutation_descriptor = mutation_functor.get_last_mutation_descriptor();
		}
		// std::cout << ", Mutated genome: " << mutated;

		double this_energy;
		if constexpr (requires_recounting_support) {
			this_energy = energy_functor(p, &last_mutation_descriptor, last_energy);
		}
		else {
			this_energy = energy_functor(mutated);
		}
		double dE = this_energy - last_energy;

		if (dE <= 0 or std::exp(-dE / temperature) > pythonic_random()) {
			accepted_gene_count++;
			// std::cout << " => Mutated Genome accepted!";

			p = mutated;
			last_energy = this_energy;

			if (this_energy < best_energy) {
				iteration_of_last_improvement = iteration;

				best_genome = mutated;
				best_energy = this_energy;
			}

			if (this_energy <= params.exiting_value) {
				has_finished_ahead_of_schedule = true;
				iterations_if_finished_early = iteration;
				break;
			}
		}
		else {
			// No improvement:
			if (params.resurrect_after_iterations and iteration_of_last_improvement != size_t(-1)) {
				// Maybe reset!
				size_t iterations_from_last_change = iteration - iteration_of_last_improvement;
				if (iterations_from_last_change >= *params.resurrect_after_iterations) {
					std::cout << "Iteration " << iteration << ": resurrecting genome from iteration " << iteration_of_last_improvement << std::endl;

					// Resurrect best genome:
					last_energy = best_energy;
					p = best_genome;

					// Say that there is no need for soon change
					iteration_of_last_improvement = iteration;

				}
			}
		}


		if (for_vague_history) {
			for_vague_history->push_back(this_energy);
		}
		if (for_usual_history) {
			for_usual_history->push_back(last_energy);
		}
		if(for_best_history) {
			for_best_history->push_back(best_energy);
		}
		// std::cout << std::endl;
	}

	if (output_debug_information) {
		std::cout << "[Annealing optimize]: Accepted genes: "
		          << accepted_gene_count << " / "
		          << (has_finished_ahead_of_schedule ? iterations_if_finished_early : params.iterations);
		if (has_finished_ahead_of_schedule) {
			std::cout
					<< " (has finished before schedule: only " << iterations_if_finished_early << " of "
					<< params.iterations << " iterations are performed)";
		}
		std::cout << std::endl;
	}

	return { best_genome, best_energy };
}


