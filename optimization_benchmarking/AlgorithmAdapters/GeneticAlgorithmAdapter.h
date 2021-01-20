//
// Created by Vova on 08.12.2020.
//

#pragma once

#include <optimization_benchmarking/optimization_algorithm.h>
#include <GA/GA_optimizer.h>

class GeneticAlgorithmAdapter : public OptimizationAlgorithm
{
public:
	GeneticAlgorithmAdapter (std::function<double (const GA::Genome&)> fitness_function,
	                         std::vector<std::pair<double, double>> point_ranges, GA::continuous_GA_params params,
	                         double _epoch_pow);

	void set_computations (size_t computations) override;
	void set_exiting_value (double value) override;
	void run () override;
	bool success () override;
	std::vector<std::pair<size_t, double>> get_target_function_dynamic () override;

private:
	std::function<double (const GA::Genome&)> temp_fitness_function;
	std::vector<std::pair<double, double>> temp_point_ranges;
	GA::continuous_GA_params temp_params;
	double epoch_pow {};
	size_t computed_epoch_number {};

	std::shared_ptr<GA::GA_optimizer> optimizer {}; // There are some problems with default construction of GA_optimizer itself

	bool m_success = false;
};


