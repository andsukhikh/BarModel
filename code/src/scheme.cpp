#include "scheme.h"
#include "solver.h"
#include "matrix.h"
#include "enums.h"
#include "boundary.h"

#include <stdexcept>
#include <iostream>
#include <bitset>
#include <algorithm>


void ExplicitScheme<ProblemDimension::_2D>::evaluate_impl()
{	
	if (!check_criterion()) throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh or a small time step");

	init();
	init_geom();

	for (std::size_t y_index = 0, y_end_index = temp.size_y(); y_index != y_end_index; ++y_index)
	{
		for (std::size_t x_index = 0, x_end_index = temp.size_x(); x_index != x_end_index; ++x_index)
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
	std::swap(temp, new_temp);
}


void ExplicitScheme<ProblemDimension::_2D>::init()
{
	!(flag_	& not_copied)
						? (new_temp = temp)
						: (temp = temp);

	flag_ &= ~not_copied;
	flag_ |= copied;
}


void ExplicitScheme<ProblemDimension::_2D>::init_geom()
{
	x_step = geometry.value().get<Length>() / grid.value().size_x();
	y_step = geometry.value().get<Width>() / grid.value().size_y();
}

void ExplicitScheme<ProblemDimension::_2D>::inner_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::lower_left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = - boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step / 2;
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = - boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step / 2;
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::upper_left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = - boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step / 2;
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step / 2;

	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::upper_right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step / 2;

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step / 2;
	
	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::lower_right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step / 2;

	auto down_flux = - boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step / 2;
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));
	
	new_temp(x, y) = temp(x, y) + 4 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::left_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = - boundary_conditions->left_boundary->at_point(temp(x, y)) * y_step;
	auto right_flux = prop->thermal_conductivity * y_step / x_step * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::right_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step * (temp(x - 1, y) - temp(x, y));
	auto right_flux = boundary_conditions->right_boundary->at_point(temp(x, y)) * y_step;

	auto down_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y - 1) - temp(x, y));
	auto up_flux = prop->thermal_conductivity * x_step / y_step / 2 * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::up_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y - 1) - temp(x, y));
	auto up_flux = boundary_conditions->up_boundary->at_point(temp(x, y)) * x_step;

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

void ExplicitScheme<ProblemDimension::_2D>::down_explicit_scheme(std::size_t x, std::size_t y)
{
	auto CONST = time_step.value() / (x_step * y_step * prop->heat_capacity * prop->density);

	auto left_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x - 1, y) - temp(x, y));
	auto right_flux = prop->thermal_conductivity * y_step / x_step / 2 * (temp(x, y) - temp(x + 1, y));

	auto down_flux = - boundary_conditions->down_boundary->at_point(temp(x, y)) * x_step;
	auto up_flux = prop->thermal_conductivity * x_step / y_step * (temp(x, y) - temp(x, y + 1));

	new_temp(x, y) = temp(x, y) + 2 * CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend.value() * (time_step.value() / (prop->heat_capacity * prop->density));
}

const bool ExplicitScheme<ProblemDimension::_2D>::check_criterion() const
{
	return time_step.value() / (x_step * x_step) + time_step.value() / (y_step * y_step) < 1 / (2 * prop->thermal_diffusivity);
}


void ImplicitScheme<ProblemDimension::_2D>::evaluate_impl()
{
	Geometry length
	{
		Length{geometry.value().get<Length>()}
	};

	Geometry width
	{
		Length{geometry.value().get<Width>()}
	};

	Boundary conditions_y
	{
		.left_boundary		= boundary_conditions.value().down_boundary,
		.right_boundary		= boundary_conditions.value().up_boundary
	};

	Boundary conditions_x
	{
		.left_boundary		= boundary_conditions.value().left_boundary,
		.right_boundary		= boundary_conditions.value().right_boundary
	};

	for (std::size_t x_index = 0, x_index_end = temp.size_x(); x_index != x_index_end; ++x_index)
	{
		Solver<ImplicitScheme<ProblemDimension::_1D>>()		.set_boundary_conditions(conditions_y)
																			.set_grid(temp.slice_y(x_index))
																			.set_geometry(length)
																			.set_initial_values(temp_init.value())
																			.set_prop(prop.value())
																			.set_Q_extend(Q_extend.value())
																			.set_time_end(time_end.value())
																			.set_time_step(time_step.value() / 2)
																			.solve(without_output);
	}

	for (std::size_t y_index = 0, y_index_end = temp.size_y(); y_index != y_index_end; ++y_index)
	{
		Solver<ImplicitScheme<ProblemDimension::_1D>>()		.set_boundary_conditions(conditions_x)
																			.set_grid(grid.value().slice_x(y_index))
																			.set_geometry(width)
																			.set_initial_values(temp_init.value())
																			.set_prop(prop.value())
																			.set_Q_extend(Q_extend.value())
																			.set_time_end(time_end.value())
																			.set_time_step(time_step.value() / 2)
																			.solve(without_output);
	}
}


void ImplicitScheme<ProblemDimension::_1D>::evaluate_impl()
{
	init_step();

	auto dim = grid.value().size();
	auto fourier_number = prop->thermal_conductivity / prop->density / prop->heat_capacity * time_step.value() / step;

	Tridiagonal matrix(dim);
	std::vector<double> free_column(dim);

	matrix.set_solution_method(ShuttleMethod());

	matrix.fill_down_diagonal(-fourier_number);
	matrix.fill_main_diagonal(1 + 2 * fourier_number);
	matrix.fill_upper_diagonal(-fourier_number);
	std::transform(grid.value().cbegin(), grid.value().cend(), free_column.begin(), 
		[&](double temp)
		{
			return temp + time_step.value() / prop.value().density / prop.value().heat_capacity * Q_extend.value(); 
		}
	);

	// if (typeid(boundary_conditions.value().left_boundary) == typeid(FirstBoundaryCondition))
	// {
	// 	auto first_coef 	= 0;
	// 	auto second_coef 	= 1;
	// 	auto third_coef 	= 0;

	// 	matrix.set_first_coefficient(first_coef, second_coef, third_coef);
	// 	free_column.front() = boundary_conditions.value().left_boundary->temperature();
	// }

	// if (typeid(boundary_conditions.value().right_boundary) == typeid(FirstBoundaryCondition))
	// {
	// 	auto first_coef 	= 0;
	// 	auto second_coef 	= 1;
	// 	auto third_coef 	= 0;

	// 	matrix.set_last_coefficient(first_coef, second_coef, third_coef);
	// 	free_column.back() = boundary_conditions.value().left_boundary->temperature();
	// }



	if (typeid(*boundary_conditions.value().left_boundary) == typeid(SecondBoundaryCondition))
	{
		auto first_coef 	= 0;
		auto second_coef 	= 1 + 2 * fourier_number;
		auto third_coef 	= -2 * fourier_number;

		matrix.set_first_coefficient(first_coef, second_coef, third_coef);
		// free_column.front() = grid.value().front();
		free_column.front() = grid.value().front() 
							+ 2 * fourier_number * step / prop->thermal_conductivity * boundary_conditions->left_boundary->at_point(temp.front()) 
							+ Q_extend.value() * time_step.value() / prop->density / prop->heat_capacity;
	}

	if (typeid(*boundary_conditions.value().right_boundary) == typeid(SecondBoundaryCondition))
	{
		auto first_coef 	= -2 * fourier_number;
		auto second_coef 	= 1 + 2 * fourier_number;
		auto third_coef 	= 0;

		matrix.set_last_coefficient(first_coef, second_coef, third_coef);
		free_column.back() = grid.value().back() 
							+ 2 * fourier_number * step / prop->thermal_conductivity * boundary_conditions->left_boundary->at_point(temp.back()) 
							+ Q_extend.value() * time_step.value() / prop->density / prop->heat_capacity;
	}



	if (typeid(*boundary_conditions.value().left_boundary) == typeid(ThirdBoundaryCondition))
	{
		auto bio_coeff 		= boundary_conditions->left_boundary->heat_transfer_coeff() * step / prop->thermal_conductivity;

		auto first_coef 	= 0;
		auto second_coef 	= 1 + 2 * fourier_number +  2 * fourier_number * bio_coeff;
		auto third_coef 	= -2 * fourier_number;

		matrix.set_first_coefficient(first_coef, second_coef, third_coef);
		free_column.front() = grid.value().front() 
							+ 2 * fourier_number * bio_coeff * boundary_conditions->left_boundary->external_temperature()
							+ Q_extend.value() * time_step.value() / prop->density / prop->heat_capacity;
	}

	if (typeid(*boundary_conditions.value().right_boundary) == typeid(ThirdBoundaryCondition))
	{
		auto bio_coeff 		= boundary_conditions->left_boundary->heat_transfer_coeff() * step / prop->thermal_conductivity;

		auto first_coef 	= -2 * fourier_number;
		auto second_coef 	= 1 + 2 * fourier_number +  2 * fourier_number * bio_coeff;
		auto third_coef 	= 0;

		matrix.set_last_coefficient(first_coef, second_coef, third_coef);
		free_column.back() = grid.value().back() 
							+ 2 * fourier_number * bio_coeff * boundary_conditions->left_boundary->external_temperature()
							+ Q_extend.value() * time_step.value() / prop->density / prop->heat_capacity;
	}


	auto temp = std::move(matrix.solve(free_column));
	double afdaf = 0.0;
}

void ImplicitScheme<ProblemDimension::_1D>::init_step()
{
	step = geometry.value().get<Length>() / grid.value().size();
}

