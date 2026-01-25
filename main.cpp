#include "solver.h"

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
	grid.show();
}