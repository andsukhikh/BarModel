#include "solver.h"
#include "matrix.h"

#include <iostream>

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
		//auto temp = Solver<ExplicitScheme>()	.set_grid(grid)
		//										.set_boundary_conditions(conditions)
		//										.set_prop(thermal_prop)
		//										.set_Q_extend(0)
		//										.set_initial_values(373)
		//										.set_time_end(2000)
		//										.set_time_step(10)
		//										.solve();
		//temp.to_Celsius_deg().show(1);

		Matrix<3, 3> matrix;
		matrix(0, 0) = 100;
		matrix(1, 1) = 100;
		matrix(2, 2) = 100;
		auto main = matrix.slice("main");
		main[0].get() = 666;
		matrix(0, 1) = 14;
		matrix(1, 2) = 88;
		auto upper = matrix.slice("upper");
		matrix(1, 0) = 2;
		matrix(2, 1) = 28;
		auto lower = matrix.slice("lower");

		auto row = matrix.row(0);
		auto column = matrix.column(0);

		auto sada = 1.1;

		std::vector b = {1.0, 2.0, 3.0};

		matrix.use_solve_method(std::shared_ptr<TridiagonalMatrix<3>>()).solve(b);

	}
	catch (const std::exception& exception)
	{
		std::clog << exception.what() << "\n";

		return EXIT_FAILURE;
	}
}