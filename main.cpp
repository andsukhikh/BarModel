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

	//std::cout << std::boolalpha << grid.is_down(0, 0) << "\n";
	//std::cout << std::boolalpha << grid.is_up(0, 10) << "\n";

	//std::cout << std::boolalpha << grid.is_left(0, 0) << "\n";
	//std::cout << std::boolalpha << grid.is_right(10, 10) << "\n";

	//std::cout << std::boolalpha << grid.is_lower_left(0, 0) << "\n";
	//std::cout << std::boolalpha << grid.is_upper_left(0, 10) << "\n";

	//std::cout << std::boolalpha << grid.is_upper_right(10, 10) << "\n";
	//std::cout << std::boolalpha << grid.is_lower_right(10, 0) << "\n";

	grid.show();
}