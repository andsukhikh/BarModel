#include "scheme.h"

#include <stdexcept>

Temperature ExplicitScheme::evaluate()
{
	x_step = prop->length / temp->size_x();
	y_step = prop->width / temp->size_y();

	if (check_criterion())
	{
		for (std::size_t y_index = 0, y_end_index = temp->size_y() - 1; y_index != y_end_index; ++y_index)
		{
			for (std::size_t x_index = 0, x_end_index = temp->size_x() - 1; x_index != x_end_index; ++x_index)
			{
				if (temp->is_lower_left(x_index, y_index)) ;
			}
		}
	}
	else throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh");
}

const bool ExplicitScheme::check_criterion() const
{
	auto time_step = time_end / time_partitions;

	return time_step / (x_step * x_step) + time_step / (y_step * y_step) < 1 / (2 * prop->thermal_diffusivity);
}