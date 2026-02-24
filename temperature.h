#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "grid.h"

#include <memory>
#include <type_traits>

enum ConvertFlags : char
{
	is_Celsius =	0b0000'0001,
	is_Kelvin =		0b0000'0010
};


class Temperature : public RegularGrid
{
	std::underlying_type_t<ConvertFlags> flag_ = is_Kelvin;
public:
	Temperature() = default;
	Temperature(std::size_t x_partitions_number, std::size_t y_partitions_number);

	Temperature(const RegularGrid& grid);

	void fill(double values);
	void show(std::size_t precision = 0);

	Temperature& to_Kelvin_deg();
	Temperature& to_Celsius_deg();
};

#endif // !TEMPERATURE_H

