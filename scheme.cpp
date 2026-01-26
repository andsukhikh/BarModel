#include "scheme.h"

#include <stdexcept>

void ExplicitScheme::evaluate()
{
	x_step = prop->length / temp->size_x();
	y_step = prop->width / temp->size_y();
	auto time_step = time_end / time_partitions;

	CONST = time_step / (x_step * y_step * prop->heat_capacity * prop->density);

	//auto explicit_scheme_lower_left = std::make_shared<ExplicitSchemeInner>(*this);
	//set_strategy(explicit_scheme_lower_left).use_strategy(x_index, y_index);

	if (check_criterion())
	{
		for (std::size_t y_index = 0, y_end_index = temp->size_y() - 1; y_index != y_end_index; ++y_index)
		{
			for (std::size_t x_index = 0, x_end_index = temp->size_x() - 1; x_index != x_end_index; ++x_index)
			{
				if (temp->is_lower_left(x_index, y_index));
				if (temp->is_upper_left(x_index, y_index));
				if (temp->is_upper_right(x_index, y_index));
				if (temp->is_lower_right(x_index, y_index));

				if (temp->is_lower_left(x_index, y_index));
				if (temp->is_upper_left(x_index, y_index));
				if (temp->is_upper_right(x_index, y_index));
				if (temp->is_lower_right(x_index, y_index));

				inner_explicit_scheme(x_index, y_index);
			}
		}

		temp.swap(new_temp);
	}
	else throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh");
}

void ExplicitScheme::inner_explicit_scheme(std::size_t i, std::size_t j)
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

ExplicitSchemeSwitcher& ExplicitScheme::set_strategy(std::shared_ptr<IExplicitScheme> strategy)
{
	strategy_ = strategy;

	return *this;
}

void ExplicitScheme::use_strategy(std::size_t i, std::size_t j)
{
	strategy_->evaluate(i, j);
}




ExplicitSchemeInner::ExplicitSchemeInner(ExplicitScheme scheme)
	:scheme_(std::make_shared<ExplicitScheme>(scheme))
{}


void ExplicitSchemeInner::evaluate(std::size_t i, std::size_t j)
{
	auto left_flux = scheme_->prop->thermal_conductivity * scheme_->y_step / scheme_->x_step * (scheme_->temp->along_y(j).along_x(i - 1) - scheme_->temp->along_y(j).along_x(i));
	auto right_flux = scheme_->prop->thermal_conductivity * scheme_->y_step / scheme_->x_step * (scheme_->temp->along_y(j).along_x(i) - scheme_->temp->along_y(j).along_x(i + 1));

	auto down_flux = scheme_->prop->thermal_conductivity * scheme_->x_step / scheme_->y_step * (scheme_->temp->along_y(j - 1).along_x(i) - scheme_->temp->along_y(j).along_x(i));
	auto up_flux = scheme_->prop->thermal_conductivity * scheme_->x_step / scheme_->y_step * (scheme_->temp->along_y(j).along_x(i) - scheme_->temp->along_y(j + 1).along_x(i));

	scheme_->new_temp->along_y(j).along_x(i) = scheme_->temp->along_y(j).along_x(i) + scheme_->CONST * (left_flux - right_flux + down_flux - up_flux) + scheme_->Q_extend;
}



