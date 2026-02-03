#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "grid.h"

#include <memory>

enum ConvertFlags : char
{
	is_Celsium =	0b0000'0001,
	is_Kelvin =		0b0000'0010
};


class Temperature : public RegularGrid
{
	ConvertFlags flag_ = is_Celsium;
public:
	Temperature() = default;
	Temperature(std::size_t x_partitions_number, std::size_t y_partitions_number);

	Temperature(const RegularGrid& grid);

	void fill(double values);
	void to_Kelvin_deg();
	void to_Celsius_deg();
	void show(std::size_t precision = 0);
};

#endif // !TEMPERATURE_H

