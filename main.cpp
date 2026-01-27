#include "solver.h"

#include <iostream>
#include <iomanip>

int main()
{
	Properties thermal_prop
	{
		.density = 9000,
		.heat_capacity = 400,
		.length = 0.6,
		.width = 0.6,
		.thermal_conductivity = 400
	};

	RegularGrid grid(11, 11);
	//grid.show();
	Boundary conditions
	{
		.left_boundary =		std::make_shared<SecondBoundaryCondition>(0),
		.right_boundary =		std::make_shared<ThirdBoundaryCondition>(0, 20),
		.up_boundary =			std::make_shared<SecondBoundaryCondition>(40000),
		.down_boundary =		std::make_shared<SecondBoundaryCondition>(20000)
	};

	try
	{
		Temperature temp = Solver()		.set_scheme(std::make_shared<ExplicitScheme>())
										.set_grid(grid)
										.set_boundary_conditions(conditions)
										.set_prop(thermal_prop)
										.set_Q_extend(1000)
										.set_initial_values(100)
										.set_time_end(2000)
										.set_time_partitions(2000)
										.solve();

		for (int j = temp.size_y() - 1; j >= 0; j--)
		{
			for (int i = 0; i <= temp.size_x() - 1; i++)
			{
				std::cout.setf(std::ios::fixed);
				std::cout.precision(1);
				std::cout << std::setw(5) << temp(i, j) << "  ";
			}

			std::cout << std::endl;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << "\n";
	}
}