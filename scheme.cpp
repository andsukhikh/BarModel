#include "scheme.h"

#include <stdexcept>


void ExplicitScheme::evaluate()
{
	CONST = time_step / (x_step * y_step * prop->heat_capacity * prop->density);

	if (check_criterion())
	{
		for (std::size_t x_index = 0, x_end_index = temp.size_x(); x_index != x_end_index; ++x_index)
		{
			for (std::size_t y_index = 0, y_end_index = temp.size_y(); y_index != y_end_index; ++y_index)
			{
				if (temp.is_lower_left(x_index, y_index))			lower_left_explicit_scheme(x_index, y_index);
				else if (temp.is_upper_left(x_index, y_index))		upper_left_explicit_scheme(x_index, y_index);
				else if (temp.is_upper_right(x_index, y_index))		upper_right_explicit_scheme(x_index, y_index);
				else if (temp.is_lower_right(x_index, y_index))		lower_right_explicit_scheme(x_index, y_index);

				else if (temp.is_left(x_index, y_index))			left_explicit_scheme(x_index, y_index);
				else if (temp.is_up(x_index, y_index))				up_explicit_scheme(x_index, y_index);
				else if (temp.is_right(x_index, y_index))			right_explicit_scheme(x_index, y_index);
				else if (temp.is_down(x_index, y_index))			down_explicit_scheme(x_index, y_index);

				else inner_explicit_scheme(x_index, y_index);
			}
		}
		temp = new_temp;
	}
	else throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh or a small time step");
}

void ExplicitScheme::inner_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::lower_left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step / 2;
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step / 2;
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::upper_left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step / 2;
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step / 2;

	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::upper_right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step / 2;

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step / 2;
	
	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::lower_right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step / 2;

	auto down_flux = boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step / 2;
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));
	
	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step;
	auto right_flux = prop->thermal_conductivity * y_step / x_step * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step;

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::up_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step;

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

void ExplicitScheme::down_explicit_scheme(std::size_t x, std::size_t y)
{
	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step;
	auto up_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend * (time_step / (prop->heat_capacity * prop->density));
}

const bool ExplicitScheme::check_criterion() const
{
	return time_step / (x_step * x_step) + time_step / (y_step * y_step) < 1 / (2 * prop->thermal_diffusivity);
}

//ExplicitSchemeSwitcher& ExplicitScheme::set_strategy(std::shared_ptr<IExplicitScheme> strategy)
//{
//	strategy_ = strategy;
//
//	return *this;
//}
//
//void ExplicitScheme::use_strategy(std::size_t i, std::size_t j)
//{
//	strategy_->evaluate(i, j);
//}




//ExplicitSchemeInner::ExplicitSchemeInner(ExplicitScheme scheme)
//	:scheme_(std::make_shared<ExplicitScheme>(scheme))
//{}
//
//
//void ExplicitSchemeInner::evaluate(std::size_t i, std::size_t j)
//{
//	//auto left_flux = scheme_->prop->thermal_conductivity * scheme_->y_step / scheme_->x_step * (scheme_->temp->along_y(j).along_x(i - 1) - scheme_->temp->along_y(j).along_x(i));
//	//auto right_flux = scheme_->prop->thermal_conductivity * scheme_->y_step / scheme_->x_step * (scheme_->temp->along_y(j).along_x(i) - scheme_->temp->along_y(j).along_x(i + 1));
//
//	//auto down_flux = scheme_->prop->thermal_conductivity * scheme_->x_step / scheme_->y_step * (scheme_->temp->along_y(j - 1).along_x(i) - scheme_->temp->along_y(j).along_x(i));
//	//auto up_flux = scheme_->prop->thermal_conductivity * scheme_->x_step / scheme_->y_step * (scheme_->temp->along_y(j).along_x(i) - scheme_->temp->along_y(j + 1).along_x(i));
//
//	//scheme_->new_temp->along_y(j).along_x(i) = scheme_->temp->along_y(j).along_x(i) + scheme_->CONST * (left_flux - right_flux + down_flux - up_flux) + scheme_->Q_extend;
//}



