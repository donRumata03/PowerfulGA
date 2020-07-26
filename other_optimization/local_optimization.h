#pragma once

#include <pythonic.h>

constexpr double default_little_delta = 1e-10;
constexpr double default_second_little_delta = 1e-6;

std::vector<double> get_gradient(const std::function<double(const std::vector<double>&)>& func, const std::vector<double>& _point, double little_delta);
std::vector<double> get_second_gradient(const std::function<double(const std::vector<double>&)>& func, const std::vector<double>& _point, double little_delta);



/// Without knowing derivative:
std::pair < double, std::vector <double> > gradient_optimize(
		const std::function<double(const std::vector<double>&)>& func,
		const std::vector<double>& start_point,
		double learning_rate, size_t iterations,
		const std::optional<std::function<void (size_t iteration, std::vector<double> current_value)>>& logging_callback = std::nullopt,
		double little_delta = default_little_delta);

std::pair < double, std::vector <double> > newton_optimize(
		const std::function<double(const std::vector<double>&)>& func,
		const std::vector<double>& start_point,
		double learning_rate, size_t iterations,
		const std::optional<std::function<void (size_t iteration, std::vector<double> current_value, double current_error_function)>>& logging_callback = std::nullopt,
		double little_delta = default_little_delta);


/// With knowing derivative:
std::pair < double, std::vector <double> > gradient_optimize(
		const std::function<double(const std::vector<double>&)>& func,
		const std::function<std::vector<double> (const std::vector<double>&)>& derivative,
		const std::vector<double>& start_point,
		double learning_rate, size_t iterations,
		const std::optional<std::function<void (size_t iteration, std::vector<double> current_value)>>& logging_callback = std::nullopt
);

std::pair < double, std::vector <double> > newton_optimize(
		const std::function<double(const std::vector<double>&)>& func,
		const std::function<std::vector<double> (const std::vector<double> &)> &derivative,
		const std::function<std::vector<double> (const std::vector<double>&)>& second_derivative,
		const std::vector<double>& start_point,
		double learning_rate, size_t iterations,
		const std::optional<std::function<void (size_t iteration, std::vector<double> current_value, double current_error_function)>>& logging_callback = std::nullopt
);

