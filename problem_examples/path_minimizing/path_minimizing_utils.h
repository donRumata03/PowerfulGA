//
// Created by vova on 14.12.2020.
//


#pragma once

#include <pythonic.h>

#include <utility>


//////////////////////////////    Utils for Utils:  //////////////////////////////

template<class output_element>
std::vector<output_element> index_to_point_path(const std::vector<size_t>& index_sequence, const std::vector<output_element>& encoding);


//////////////////////////////    Generating:  //////////////////////////////
std::vector<size_t> generate_path(size_t number);


//////////////////////////////    Counting / Error function:  //////////////////////////////
double count_path_length(const std::vector<point>& points);

double fitness_by_distance (double distance);
double error_by_distance (double distance);

double distance_by_fitness (double fitness);
double distance_by_error (double error);


/// Just counts path length each time:
class DummyPathLengthCounter
{
	std::vector<point> encoding;

public:
	explicit DummyPathLengthCounter(std::vector<point> _encoding) : encoding(std::move(_encoding)) {}

	double operator () (const std::vector<size_t>& index_sequence);
};


/// Is able to cleverly recount path after certain types of mutations:
class PathLengthCounter
{
	double current_path_length {};

public:
	explicit PathLengthCounter(const std::vector<point>& points) {
		current_path_length = count_path_length(points);
	}

	[[nodiscard]] double get_path_length() const { return current_path_length; }

	void swap(size_t index1, size_t index2);
	void move(size_t index_from, size_t index_to);
	void reverse(size_t index_from, size_t index_to);
};

//////////////////////////////    Mutations:  //////////////////////////////

double transfer_range(double value, std::pair<double, double> from_range, std::pair<double, double> to_range);
size_t distance_by_amount (double amount, size_t size);
size_t generate_start_point(size_t size, size_t distance);
std::pair<size_t, size_t> generate_range(size_t size, size_t distance);

template<class genome_element>
void swapping_mutate(std::vector<genome_element>& points, size_t distance);

template<class genome_element>
void moving_mutate(std::vector<genome_element>& points, size_t distance);

template<class genome_element>
void reversing_mutate(std::vector<genome_element>& points, size_t distance);

template<class genome_element>
std::vector<genome_element> mutate_path(const std::vector<genome_element>& points, double amount);


//////////////////////////////	 Implementations:	//////////////////////

template<class genome_element>
void swapping_mutate (std::vector<genome_element>& points, size_t distance)
{
	// size_t start_point = generate_start_point(points.size(), distance);
	// size_t end_point = start_point + distance;

	auto [start_point, end_point] = generate_range(points.size(), distance);
	std::swap(points[start_point], points[end_point]);
}


template<class genome_element>
void moving_mutate (std::vector<genome_element>& points, size_t distance)
{
	auto [start_point, end_point] = generate_range(points.size(), distance);
	bool direction = bool(randint(0, 102341) % 2);
//	std::cout
//		<< "Direction: " << std::boolalpha << direction
//		<< " Range: [" << start_point << "; " << end_point << "]"
//	<< std::endl;

	if (direction) {
		// Move the left one to the right corner:
		auto old_element = points[start_point];
		std::copy(
				points.begin() + start_point + 1,
				points.begin() + end_point + 1,
				points.begin() + start_point);
		points[end_point] = old_element;
	}
	else {
		// Move the right one to the left corner:
		auto old_element = points[end_point];

		for (li old_index = li(end_point - 1); old_index >= li(start_point); old_index--) {
			points[old_index + 1] = points[old_index];
		}

		points[start_point] = old_element;
	}
}

template<class genome_element>
void reversing_mutate (std::vector<genome_element>& points, size_t distance)
{
	auto [start_point, end_point] = generate_range(points.size(), distance);

	// Debug: // std::cout << "Reversing from " << start_point << " to " << end_point << std::endl;

	std::reverse(points.begin() + start_point, points.begin() + end_point + 1);
}


template<class genome_element>
std::vector<genome_element> mutate_path (const std::vector<genome_element>& points, double amount)
{
	auto res = points;
	auto dist = distance_by_amount(amount, points.size());

	if (pythonic_random() < 0.5) {
		moving_mutate(res, dist);
	}
	else {
		reversing_mutate(res, dist);
	}

	assert(res.size() == points.size());

	return res;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class output_element>
std::vector<output_element> index_to_point_path (const std::vector<size_t>& index_sequence, const std::vector<output_element>& encoding)
{
	std::vector<output_element> res(index_sequence.size());
	for (size_t i = 0; i < index_sequence.size(); ++i) {
		assert(index_sequence[i] < encoding.size());
		res[i] = encoding[index_sequence[i]];
	}

	return res;
}


