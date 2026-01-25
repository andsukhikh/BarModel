#ifndef GRID_H
#define GRID_H

#include <vector>

//  	 X	
//  	^
//   3	| 1 - 2 - 3 - 4 - 5
//   	| |   |   |   |   |
//   2	| 1 - 2 - 3 - 4 - 5
//   	| |   |   |   |   |
//   1	| 1 - 2 - 3 - 4 - 5
//  	*--------------------> Y

class ProxyArray
{
	std::vector<double> x_axis;
public:
	ProxyArray(std::size_t total_number_of_point_x_axis);

	double& along_x(std::size_t index);

	std::vector<double>::iterator begin();

	std::vector<double>::iterator end();

	std::vector<double>::const_iterator cbegin() const;

	std::vector<double>::const_iterator cend() const;

	const std::size_t size() const;
};


class RegularGrid
{
public:
	std::vector<ProxyArray> y_axis;
public:
	RegularGrid(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis);

	ProxyArray& along_y(std::size_t index);

	void show();

	const std::size_t size_x() const;

	const std::size_t size_y() const;

	const bool is_lower_left(std::size_t x_index, std::size_t y_index) const;

	const bool is_upper_left(std::size_t x_index, std::size_t y_index) const;

	const bool is_upper_right(std::size_t x_index, std::size_t y_index) const;

	const bool is_lower_right(std::size_t x_index, std::size_t y_index) const;


	const bool is_left(std::size_t x_index, std::size_t y_index) const;

	const bool is_right(std::size_t x_index, std::size_t y_index) const;

	const bool is_up(std::size_t x_index, std::size_t y_index) const;

	const bool is_down(std::size_t x_index, std::size_t y_index) const;

private:
	const std::size_t count_digit(std::size_t number) const;
};


#endif GRID_H
