#ifndef PROPERTIES_H
#define PROPERTIES_H

struct Properties
{
	const double density;
	const double heat_capacity;
	const double length;
	const double width;
	const double thermal_conductivity;

	const double thermal_diffusivity = thermal_conductivity / (heat_capacity * density);
};

#endif // !PROPERTIES_H

