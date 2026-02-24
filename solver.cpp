#include "solver.h"

#include <optional>
#include <stdexcept>
#include <iostream>


template class Solver<ExplicitScheme>;
template class Solver<ImplicitScheme>;


template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_prop(const Properties& prop) &&
{
    this->prop.emplace(prop);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_grid(const RegularGrid& grid) &&
{
    this->grid      = std::make_optional<RegularGrid>(grid);
    this->temp      = Temperature(grid);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_boundary_conditions(const Boundary& boundary_conds) && 
{
    this->boundary_conditions = std::make_optional<Boundary>(boundary_conds);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_end(double time_end) &&
{
    this->time_end = std::make_optional<double>(time_end);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_step(std::size_t time_step) &&
{
    this->time_step = std::make_optional<double>(time_step);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_Q_extend(double Q_extend) &&
{
    this->Q_extend = Q_extend;
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_initial_values(double temp_init) &&
{
    this->temp_init.emplace(temp_init);
    this->temp.fill(temp_init);
    return std::move(*this);
}

template<typename Scheme>
Temperature Solver<Scheme>::solve() &&
{
    if (!this->Q_extend)
    {
        std::clog << "Energy release is not specified. The default value of 0 J/m^3 is assumed" << "\n" << std::endl;
        this->Q_extend.emplace(0.0);
    }

    if (!this->temp_init)
    {
        std::clog << "Initial conditions are not specified.The default value is set to 273 K" << "\n" << std::endl;
        this->temp.fill(Properties::deg_Kelvin_is);
    }

    if (!this->grid                     .has_value())   throw std::runtime_error("There is no grid. Please set the grid option.");
    if (!this->prop                     .has_value())   throw std::runtime_error("There are no thermophysical properties of the calculated body");
    if (!this->boundary_conditions      .has_value())   throw std::runtime_error("There are no boundary conditions parameters");
    if (!this->time_end                 .has_value())   throw std::runtime_error("The end time of the calculation is not specified.");
    if (!this->time_step                .has_value())   throw std::runtime_error("Time step not specified");

    this->x_step = this->prop->length / (this->temp.size_x() - 1);
    this->y_step = this->prop->width / (this->temp.size_y() - 1);

    for (std::size_t time_index = 0, end_time_index = *this->time_end / *this->time_step; time_index != end_time_index; ++time_index)
    {
        this->evaluate();
    }
    return std::move(this->temp);
}