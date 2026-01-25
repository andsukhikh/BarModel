#ifndef SCHEME_H
#define SCHEME_H

#include <memory>

#include "temperature.h"
#include "properties.h"
#include "boundary.h"


struct IScheme
{
	virtual ~IScheme() = 0;
	virtual Temperature evaluate() = 0;
};


class ExplicitScheme : public IScheme
{
public:
	std::shared_ptr<Temperature> temp;

	std::shared_ptr<Properties> prop;

	std::shared_ptr<Boundary> boundary_conditions;

	std::size_t time_partitions;
	double time_end;

	double x_step;
	double y_step;

	double Q_extend;

	std::shared_ptr<Temperature> new_temp = temp;
	double CONST;
public:
	Temperature evaluate() override;
private:
	//void calculate_at(std::size_t i, std::size_t j/*, const std::optional<BoundaryCondition>& conditions = std::nullopt*/)
	//{
	//	//if (conditions.has_value())
	//	//{
	//	//	decltype(auto) left_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i - 1) - temp->along_y(j).along_x(i));
	//	//	decltype(auto) right_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i) - temp->along_y(j).along_x(i + 1));

	//	//	decltype(auto) down_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j - 1).along_x(i) - temp->along_y(j).along_x(i));
	//	//	decltype(auto) up_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j).along_x(i) - temp->along_y(j + 1).along_x(i));

	//	//	new_temp->along_y(j).along_x(i) = temp->along_y(j).along_x(i) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend;
	//	//}

	//	decltype(auto) left_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i - 1) - temp->along_y(j).along_x(i));
	//	decltype(auto) right_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i) - temp->along_y(j).along_x(i + 1));

	//	decltype(auto) down_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j - 1).along_x(i) - temp->along_y(j).along_x(i));
	//	decltype(auto) up_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j).along_x(i) - temp->along_y(j + 1).along_x(i));

	//	new_temp->along_y(j).along_x(i) = temp->along_y(j).along_x(i) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend;
	//}

	const bool check_criterion() const;
};


class ExplicitSchemeLowerLeft : public IScheme
{
	Temperature evaluate() override;
};

#endif // !SCHEME_H
