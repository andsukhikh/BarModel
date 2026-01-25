#include "scheme.h"

#include <stdexcept>

Temperature ExplicitScheme::evaluate()
{
	x_step = prop->length / temp->size_x();
	y_step = prop->width / temp->size_y();
	auto time_step = time_end / time_partitions;

	CONST = time_step / (x_step * y_step * prop->heat_capacity * prop->density);

	if (check_criterion())
	{
		for (std::size_t y_index = 0, y_end_index = temp->size_y() - 1; y_index != y_end_index; ++y_index)
		{
			for (std::size_t x_index = 0, x_end_index = temp->size_x() - 1; x_index != x_end_index; ++x_index)
			{
				if (temp->is_lower_left(x_index, y_index)) /*set_strategy(std::make_shared<ExplicitSchemeLowerLeft>()).use_strategy();*/
				if (temp->is_upper_left(x_index, y_index));
				if (temp->is_upper_right(x_index, y_index));
				if (temp->is_lower_right(x_index, y_index));

				if (temp->is_lower_left(x_index, y_index));
				if (temp->is_upper_left(x_index, y_index));
				if (temp->is_upper_right(x_index, y_index));
				if (temp->is_lower_right(x_index, y_index));

				calculate_at(x_index, y_index);
			}
		}
	}
	else throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh");
}

void ExplicitScheme::calculate_at(std::size_t i, std::size_t j)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i - 1) - temp->along_y(j).along_x(i));
	auto right_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i) - temp->along_y(j).along_x(i + 1));

	auto down_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j - 1).along_x(i) - temp->along_y(j).along_x(i));
	auto up_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j).along_x(i) - temp->along_y(j + 1).along_x(i));

	new_temp->along_y(j).along_x(i) = temp->along_y(j).along_x(i) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend;
}

const bool ExplicitScheme::check_criterion() const
{
	auto time_step = time_end / time_partitions;

	return time_step / (x_step * x_step) + time_step / (y_step * y_step) < 1 / (2 * prop->thermal_diffusivity);
}
