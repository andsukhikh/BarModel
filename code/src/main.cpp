#include "solver.h"
#include "scheme.h"
#include "matrix.h"
#include "geometry.h"

#include <iostream>
#include <memory>
#include <array>


int main()
{
	Properties thermal_prop
	{
		.density = 9000,
		.heat_capacity = 400,
		.thermal_conductivity = 300
	};

	Geometry geometry
	(
		Length {0.6}, 
		Width  {0.6}
	);

	RegularGrid<ProblemDimension::_2D> grid(5, 10);

	Boundary conditions
	{
		.left_boundary		= std::make_shared<SecondBoundaryCondition>(0),
		.right_boundary		= std::make_shared<SecondBoundaryCondition>(0),
		.up_boundary		= std::make_shared<SecondBoundaryCondition>(10000),
		.down_boundary		= std::make_shared<SecondBoundaryCondition>(0)
	};

	try
	{
		auto temp = Solver<ExplicitScheme<ProblemDimension::_2D>>()	.set_grid(grid)
																	.set_geometry(geometry)
																	.set_boundary_conditions(conditions)
																	.set_prop(thermal_prop)
																	.set_Q_extend(0)
																	.set_initial_values(373)
																	.set_time_end(200)
																	.set_time_step(10)
																	.solve();
		temp.to_Celsius_deg().show(1);



		// auto temp_ = Solver<ImplicitScheme<ProblemDimension::_2D>>() .set_grid(grid)
		// 															.set_geometry(geometry)
		// 															.set_boundary_conditions(conditions)
		// 															.set_prop(thermal_prop)
		// 															.set_Q_extend(0)
		// 															.set_initial_values(373)
		// 															.set_time_end(200)
		// 															.set_time_step(10)
		// 															.solve();
		// temp_.to_Celsius_deg().show(1);
	}

	catch (const std::exception& exception)
	{
		std::clog << exception.what() << "\n";

		return EXIT_FAILURE;
	}
}
