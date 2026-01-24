#include <vector>
#include <iostream>
#include <memory>
#include <format>
#include <string>
#include <iomanip>
#include <exception>
#include <optional>

//  	 X	
//  	^
//   3	| 1 - 2 - 3 - 4 - 5
//   	| |   |   |   |   |
//   2	| 1 - 2 - 3 - 4 - 5
//   	| |   |   |   |   |
//   1	| 1 - 2 - 3 - 4 - 5
//  	*--------------------> Y

class ProxyArray
{
	std::vector<double> x_axis;
public:
	ProxyArray(std::size_t total_number_of_point_x_axis)
		: x_axis(total_number_of_point_x_axis)
	{}

	double& along_x(std::size_t index)
	{
		return x_axis.at(index);
	}

	decltype(auto) begin()
	{
		return x_axis.begin();
	}

	decltype(auto) end()
	{
		return x_axis.end();
	}

	decltype(auto) cbegin() const
	{
		return x_axis.cbegin();
	}

	decltype(auto) cend() const
	{
		return x_axis.cend();
	}

	const std::size_t size() const
	{
		return x_axis.size();
	}
};


class RegularGrid
{
	std::vector<ProxyArray> y_axis;
public:
	RegularGrid(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
		: y_axis(total_number_of_point_y_axis, ProxyArray(total_number_of_point_x_axis))
	{}

	ProxyArray& along_y(std::size_t index)
	{
		return y_axis.at(index);
	}

	void show()
	{
		auto max_number_digit_y = count_digit(y_axis.end() - y_axis.begin());

		std::cout	<< std::setw(max_number_digit_y) << " " << "X" << "\n"
					<< std::setw(max_number_digit_y) << " " << "^" << "\n";
		
		for (auto iterator_y = y_axis.rbegin(); iterator_y != y_axis.rend(); ++iterator_y)
		{
			auto index = y_axis.crend() - iterator_y;
			std::cout << std::setw(max_number_digit_y) << index << "|";

			for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
			{
				auto index = iterator_x - iterator_y->cbegin() + 1;

				std::cout << index;
				if (iterator_x != iterator_y->cend() - 1) std::cout << "--";
			}

			auto y_axis_indent = max_number_digit_y + 1;

			std::cout << "\n";

			if (iterator_y != y_axis.crend() - 1)
			{
				std::cout << std::setw(y_axis_indent) << "|";
				for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
				{
					auto index = iterator_x - iterator_y->cbegin() + 1;
					auto digit_number = count_digit(index);
					auto middle_pos = digit_number / 2;
					auto indent = digit_number + middle_pos;

					std::cout << "|" << " " << std::string(indent, ' ');
				}
				std::cout << "\n";
			}
		}
	}

	decltype(auto) size_x() const
	{
		return y_axis.at(0).size();
	}

	decltype(auto) size_y() const
	{
		return y_axis.size();
	}

private:
	std::size_t count_digit(std::size_t number)
	{
		auto string = std::to_string(number);

		return string.size();
	}
};


class Temperature
{
	std::shared_ptr<RegularGrid> grid_;
public:
	Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
		: grid_(std::make_shared<RegularGrid>(total_number_of_point_x_axis, total_number_of_point_y_axis))
	{}

	Temperature(std::shared_ptr<RegularGrid> grid)
		: grid_(grid)
	{}

	Temperature(const Temperature& temperature)
	{
		grid_ = temperature.grid_;
	}

	Temperature& operator=(const Temperature& temperature)
	{
		grid_ = temperature.grid_;
	}

	decltype(auto) along_y(std::size_t index)
	{
		return grid_->along_y(index);
	}

	const std::size_t size_x() const 
	{
		return grid_->size_x();
	}

	const std::size_t size_y() const
	{
		return grid_->size_y();
	}

	void set_initial_value(double init_temp)
	{
	}
};


struct BoundaryCondition
{
	virtual ~BoundaryCondition() = default;
	virtual double at_point(const double temperature) = 0;
};


class FirstBoundaryCondition : public BoundaryCondition
{
	const double external_temperature_;
public:
	FirstBoundaryCondition(double external_temperature)
		: external_temperature_(external_temperature)
	{}

	double at_point(const double temperature) override
	{
		return external_temperature_;
	}
};


class SecondBoundaryCondition : public BoundaryCondition
{
	const double flux_;
public:
	SecondBoundaryCondition(double flux)
		: flux_(flux)
	{}

	double at_point(const double temperature) override
	{
		return flux_;
	}
};


class ThirdBoundaryCondition : public BoundaryCondition
{
	const double heat_transfer_coefficient_;
	const double external_temperature_;
public:
	ThirdBoundaryCondition(double heat_transfer_coefficient, double extend_temperature)
		: heat_transfer_coefficient_(heat_transfer_coefficient)
		, external_temperature_(extend_temperature)
	{}

	double at_point(const double temperature) override
	{
		return heat_transfer_coefficient_ * (temperature - external_temperature_);
	}
};


struct Boundary
{
	std::shared_ptr<BoundaryCondition> left_boundary_;
	std::shared_ptr<BoundaryCondition> right_boundary_;
	std::shared_ptr<BoundaryCondition> up_boundary_;
	std::shared_ptr<BoundaryCondition> down_boundary_;
//public:
//	static const bool check_left_down_corner(std::size_t x_index, std::size_t y_index)
//	{
//		return 
//	}
};


struct Properties
{
	const double density;
	const double heat_capacity;
	const double length;
	const double width;
	const double thermal_conductivity;

	const double thermal_diffusivity = thermal_conductivity / (heat_capacity * density);
};


class LowerRight
{
	static const bool check(std::size_t x_index, std::size_t y_index)
	{
		return x_index == 0 && y_index == 0;
	}
};


class ExplicitScheme
{
public:
	std::shared_ptr<Temperature> temp;
	std::shared_ptr<Temperature> new_temp = temp;

	std::shared_ptr<Properties> prop;

	std::shared_ptr<Boundary> boundary_conditions;

	std::size_t time_partitions;
	double time_end;

	double x_step;
	double y_step;

	double Q_extend;

	double CONST;
public:

	Temperature evaluate()
	{
		x_step = prop->length / temp->size_x();
		y_step = prop->width / temp->size_y();

		if (check_criterion())
		{
			for (std::size_t y_index = 0, y_end_index = temp->size_y() - 1; y_index != y_end_index; ++y_index)
			{
				for (std::size_t x_index = 0, x_end_index = temp->size_x() - 1; x_index != x_end_index; ++x_index)
				{
					//if (check_left_down_corner(x_index, y_index))
					//if (check_left_up_corner(x_index, y_index))
					//if (check_right_down_corner(x_index, y_index))
					//if (check_right_up_corner(x_index, y_index))

					//calculate_at(x_index, y_index);

					if (check<LowerRight>(x_index, y_index)) calculate_at<LowerRight>(x_index, y_index);
				}
			}
		}
		else throw std::runtime_error("The convergence condition of the difference scheme is not met: coarse mesh");
	}
private:
	template<typename Position>
	void calculate_at(std::size_t i, std::size_t j/*, const std::optional<BoundaryCondition>& conditions = std::nullopt*/)
	{
		//if (conditions.has_value())
		//{
		//	decltype(auto) left_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i - 1) - temp->along_y(j).along_x(i));
		//	decltype(auto) right_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i) - temp->along_y(j).along_x(i + 1));

		//	decltype(auto) down_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j - 1).along_x(i) - temp->along_y(j).along_x(i));
		//	decltype(auto) up_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j).along_x(i) - temp->along_y(j + 1).along_x(i));

		//	new_temp->along_y(j).along_x(i) = temp->along_y(j).along_x(i) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend;
		//}

		decltype(auto) left_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i - 1) - temp->along_y(j).along_x(i));
		decltype(auto) right_flux = prop->thermal_conductivity * y_step / x_step * (temp->along_y(j).along_x(i) - temp->along_y(j).along_x(i + 1));

		decltype(auto) down_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j - 1).along_x(i) - temp->along_y(j).along_x(i));
		decltype(auto) up_flux = prop->thermal_conductivity * x_step / y_step * (temp->along_y(j).along_x(i) - temp->along_y(j + 1).along_x(i));

		new_temp->along_y(j).along_x(i) = temp->along_y(j).along_x(i) + CONST * (left_flux - right_flux + down_flux - up_flux) + Q_extend;
	}



	template<typename Position>
	const bool check(std::size_t x_index, std::size_t y_index) const
	{
		return Position::check(x_index, y_index);
	}

	const bool check_criterion() const
	{
		auto time_step = time_end / time_partitions;

		return time_step / (x_step * x_step) + time_step / (y_step * y_step) < 1 / (2 * prop->thermal_diffusivity);
	}

	//const bool check_left_down_corner(std::size_t x_index, std::size_t y_index) const
	//{
	//	return x_index == 0 && y_index == 0;
	//}

	//const bool check_left_up_corner(std::size_t x_index, std::size_t y_index) const
	//{
	//	return x_index == 0 && y_index == temp->size_y() - 1;
	//}

	//const bool check_right_down_corner(std::size_t x_index, std::size_t y_index) const
	//{
	//	return x_index == temp->size_x() - 1 && y_index == 0;
	//}

	//const bool check_right_up_corner(std::size_t x_index, std::size_t y_index) const
	//{
	//	return x_index == temp->size_x() - 1 && y_index == temp->size_y() - 1;
	//}
};


class Solver
{
	std::shared_ptr<ExplicitScheme> scheme_;
public:
	Solver() = default;

	Solver& set_prop(const Properties& prop)
	{
		scheme_->prop = std::make_shared<Properties>(prop);
		return *this;
	}

	Solver& set_grid(const RegularGrid& grid)
	{
		scheme_->temp = std::make_shared<Temperature>(grid);
		return *this;
	}

	Solver& set_boundary_conditions(const Boundary& boundary_conds)
	{
		scheme_->boundary_conditions = std::make_shared<Boundary>(boundary_conds);
		return *this;
	}

	Solver& set_time_end(const double& time_end)
	{
		scheme_->time_end = time_end;
		return *this;
	}

	Solver& set_time_partitions(const std::size_t& time_partitions)
	{
		scheme_->time_partitions = time_partitions;
		return *this;
	}

	Solver& set_Q_extend(const double& Q_extend)
	{
		scheme_->Q_extend = Q_extend;
		return *this;
	}

	std::shared_ptr<Temperature> solve()
	{
		for (std::size_t time_index = 0, end_time_index = scheme_->time_partitions; time_index != end_time_index; ++time_index)
		{
			*scheme_->temp = scheme_->evaluate();
		}

		return scheme_->temp;

	}
};


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
	Temperature temperature(std::make_shared<RegularGrid>(grid));
}