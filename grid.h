#ifndef GRID_H
#define GRID_H

#include <vector>

//  	 X	
//  	^
//   2	| 10 - 11 - 12 - 13 - 14
//   	| |    |    |    |    |
//   1	| 5 -- 6 -- 7 -- 8 -- 9
//   	| |    |    |    |    |
//   0	| 0 -- 1 -- 2 -- 3 -- 4
//  	*--------------------> Y

class RegularGrid
{
protected:
	std::vector<double> grid_;
	std::vector<std::size_t> row_number_;
public:
	RegularGrid(std::size_t x_partitions_number, std::size_t y_partitions_number);

	RegularGrid() = default;

	double& operator() (std::size_t x_index, std::size_t y_index);

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

protected:
	const std::size_t count_digit(std::size_t number) const;
};


#endif GRID_H
