#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "grid.h"

#include <memory>

class Temperature : public RegularGrid
{
public:
	Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis);

	Temperature(const RegularGrid& grid);


};

#endif // !TEMPERATURE_H

