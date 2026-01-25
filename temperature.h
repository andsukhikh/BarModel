#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "grid.h"

#include <memory>

class Temperature 
{
	std::shared_ptr<RegularGrid> grid_;
public:
	Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis);

	Temperature(const RegularGrid& grid);

	Temperature(const Temperature& temperature);

	Temperature& operator=(const Temperature& temperature);

	ProxyArray& along_y(std::size_t index);

	const std::size_t size_x() const;

	const std::size_t size_y() const;
};

#endif // !TEMPERATURE_H

