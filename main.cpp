#include "solver.h"

#include <iostream>
#include <source_location>

int main()
{
	Properties thermal_prop
	{
		.density = 9000,
		.heat_capacity = 400,
		.length = 0.6,
		.width = 0.6,
		.thermal_conductivity = 300
	};

	RegularGrid grid(11, 11);

	Boundary conditions
	{
		.left_boundary =		std::make_shared<SecondBoundaryCondition>(0),
		.right_boundary =		std::make_shared<SecondBoundaryCondition>(0),
		.up_boundary =			std::make_shared<SecondBoundaryCondition>(10000),
		.down_boundary =		std::make_shared<SecondBoundaryCondition>(0)
	};

	try
	{
		auto temp = Solver<ExplicitScheme>()	.set_grid(grid)
												.set_boundary_conditions(conditions)
												.set_prop(thermal_prop)
												.set_Q_extend(0)
												.set_initial_values(100)
												.set_time_end(2000)
												.set_time_step(10)
												.solve();
		//temp.to_Kelvin_deg();
		temp.show(1);
	}
	catch (const std::exception& exception)
	{
		std::clog << exception.what() << "\n";

		return EXIT_FAILURE;
	}
}