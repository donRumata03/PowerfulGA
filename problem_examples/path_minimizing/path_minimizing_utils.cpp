//
// Created by vova on 14.12.2020.
//

#include "path_minimizing_utils.h"
#include "pythonic.h"

std::vector<size_t> generate_path (size_t number)
{
	std::vector<size_t> res(number);
	std::iota(res.begin(), res.end(), 0);
	std::shuffle(res.begin(), res.end(), std::mt19937{ std::random_device{}() });
	return res;
}


double count_path_length (const std::vector<point>& points)
{
	double res = 0;
	for (size_t i = 1; i < points.size(); ++i) {
		res += point::dist(points[i], points[i - 1]);
	}

	return res;
}

void PathLengthCounter::swap (size_t index1, size_t index2)
{
	// TODO: use segment tree?
}

void PathLengthCounter::move (size_t index_from, size_t index_to)
{
	// TODO: use segment tree?
}


void PathLengthCounter::reverse (size_t index_from, size_t index_to)
{

}



//////////////////////////////    Mutations:  //////////////////////////////


double transfer_range (double value, std::pair<double, double> from_range, std::pair<double, double> to_range)
{
	double size_first = from_range.second - from_range.first;
	double size_second = to_range.second - to_range.first;

	return to_range.first + (size_second * (value - from_range.first) / size_first);
}

size_t distance_by_amount (double amount, size_t size)
{
	double target_distance = transfer_range(amount,
								{ 0., 1. },
								{ std::max(1., double(size) * 0.05), double(size) * 0.35 });

	double generated_distance = normal_distribute(target_distance, target_distance, 1)[0];

	auto distance = size_t(std::clamp(
			li(std::round(generated_distance))
			, 1LL, li(size) - 1));

	return distance;
}



size_t generate_start_point (size_t size, size_t distance)
{
	// Both points are from [0, size - 1]
	return randint(0, size - distance); // It's [from, to);
}

std::pair<size_t, size_t> generate_range (size_t size, size_t distance)
{
	auto first = generate_start_point(size, distance);
	return std::pair<size_t, size_t>{ first, first + distance };
}

