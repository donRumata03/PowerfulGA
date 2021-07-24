#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include "local_optimization.h"


///////////////////////////////////
//		First gradient			//
/////////////////////////////////


std::vector<double> get_gradient(const std::function<double(const std::vector<double>&)>& func,
	const std::vector<double>& _point, const double little_delta)
{
	std::vector<double> point = _point;

	size_t dimensions = point.size();
	std::vector<double> res(dimensions);

	double f0 = func(point);

	for (size_t n_dim = 0; n_dim < dimensions; n_dim++)
	{
		point[n_dim] += little_delta;

		double new_f = func(point);
		res[n_dim] = (new_f - f0) / little_delta;

		point[n_dim] -= little_delta;
	}

	return res;
}


///////////////////////////////////
//		Second gradient			//
/////////////////////////////////

std::vector<double> get_second_gradient(const std::function<double(const std::vector<double>&)>& func,
	const std::vector<double>& _point, const double little_delta)
{
	std::vector<double> point = _point;

	size_t dimensions = point.size();
	std::vector<double> res(dimensions);

	double f0 = func(point);

	for (size_t n_dim = 0; n_dim < dimensions; n_dim++)
	{
		point[n_dim] += little_delta;
		double f1 = func(point);
		double derivative1  = (f1 - f0) / little_delta;

		point[n_dim] += little_delta;
		double f2 = func(point);
		double derivative2 = (f2 - f1) / little_delta;

		double second_derivative = (derivative2 - derivative1) / little_delta;
		res[n_dim] = second_derivative;
		
		point[n_dim] -= 2 * little_delta;
	}

	return res;
}




   ////////////////////////////////////
 ///	Gradient Decent				//
/////////////////////////////////////

/// Without knowing derivative:

std::pair<double, std::vector<double>> gradient_optimize(const std::function<double(const std::vector<double>&)>& func,
	const std::vector<double>& start_point, double learning_rate, const size_t iterations,
	const std::optional<std::function<void (size_t iteration, std::vector<double> current_value)>>& logging_callback, double little_delta)
{
	return gradient_optimize(func,
	                       [little_delta, & start_point, & func](const std::vector<double>&) -> std::vector<double> { return get_gradient(func, start_point, little_delta); },
	                       start_point, learning_rate, iterations, logging_callback);
}



std::pair<double, std::vector<double>> newton_optimize(const std::function<double(const std::vector<double>&)>& func,
	const std::vector<double>& start_point, double learning_rate, size_t iterations,
	const std::optional<std::function<void (size_t iteration, std::vector<double> current_value, double current_error_function)>>& logging_callback, double little_delta)
{
	return newton_optimize(func,
			[little_delta, & start_point, & func](const std::vector<double>&) -> std::vector<double> { return get_gradient(func, start_point, little_delta); },
			[little_delta, & start_point, & func](const std::vector<double>&) -> std::vector<double> { return get_second_gradient(func, start_point, little_delta); },
			start_point, learning_rate, iterations, logging_callback);
}



/// With knowing derivative:

std::pair<double, std::vector<double> >
gradient_optimize (const std::function<double (const std::vector<double> &)> &func,
                   const std::function<std::vector<double> (const std::vector<double> &)> &derivative,
                   const std::vector<double> &start_point, double learning_rate, size_t iterations,
                   const std::optional<std::function<void (size_t iteration, std::vector<double> current_value)>>& logging_callback)
{

	auto current_point = start_point;

	size_t dimensions = start_point.size();

	for (size_t iteration = 0; iteration < iterations; iteration++)
	{
		auto this_grad = derivative(current_point);

		for(size_t n_dim = 0; n_dim < dimensions; n_dim++)
		{
			current_point[n_dim] -= learning_rate * this_grad[n_dim];
		}

		if (logging_callback)
			logging_callback->operator()(iteration, current_point);
	}

	return { func(current_point), current_point };
}

std::pair<double, std::vector<double> >
newton_optimize (const std::function<double (const std::vector<double> &)> &func,
                 const std::function<std::vector<double> (const std::vector<double> &)> &derivative,
                 const std::function<std::vector<double> (const std::vector<double>&)>& second_derivative,
                 const std::vector<double> &start_point, double learning_rate, size_t iterations, const std::optional<std::function<void (size_t iteration, std::vector<double> current_value, double current_error_function)>>& logging_callback)
{
	auto current_point = start_point;
	auto previous_point = current_point;

	double prev_function_value = std::numeric_limits<double>::infinity();

	size_t dimensions = start_point.size();


	for (size_t iteration = 0; iteration < iterations; iteration++)
	{
		double function_result;
		std::vector<double> first_grad, second_grad;

		try {
			function_result = func(current_point);
		} catch(std::exception& e) {
			return { prev_function_value, previous_point };
		}
		if (prev_function_value < function_result or std::isnan(function_result)) {
			std::cout << "[Newton Method]: at iteration " << iteration << " / " << iterations
				<< " previous result of function was smaller than this one (or this is just None…): " << prev_function_value << " → " << function_result
			<< " ==> return previous one (" << previous_point << " instead of " << current_point << ")" << std::endl;

			return { prev_function_value, previous_point };
		}

		try {
			first_grad = derivative(current_point);
			second_grad = second_derivative(current_point);
		} catch(std::exception& e) {
			return { func(current_point), current_point };
		}

		previous_point = current_point;
		prev_function_value = function_result;
		for (size_t n_dim = 0; n_dim < dimensions; n_dim++)
		{
			if (abs(first_grad[n_dim]) < 1e-120 || abs(second_grad[n_dim]) < 1e-120 || second_grad[n_dim] == 0) {
				return { func(current_point), current_point};
			}

			current_point[n_dim] -= learning_rate * first_grad[n_dim] / second_grad[n_dim];
		}

		if (logging_callback)
			logging_callback->operator()(iteration, current_point, function_result);
	}

	return { func(current_point), current_point };
}


