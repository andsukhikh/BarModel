#include "solver.h"
#include "scheme.h"
#include "geometry.h"


#include <optional>
#include <stdexcept>
#include <iostream>


template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_prop(const Properties& prop) &&
{
    this->prop.emplace(prop);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_geometry(const typename Scheme::GeomType& geom) &&
{
    this->geometry.emplace(geom);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_grid(const RegularGrid<Scheme::Rank>& grid) &&
{
    this->grid.emplace(grid);
    this->temp = std::move(Temperature(*this->grid));
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_boundary_conditions(const Boundary& boundary_conds) && 
{
    this->boundary_conditions.emplace(boundary_conds);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_end(double time_end) &&
{
    this->time_end.emplace(time_end);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_step(std::size_t time_step) &&
{
    this->time_step.emplace(time_step);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_Q_extend(double Q_extend) &&
{
    this->Q_extend.emplace(Q_extend);
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
Temperature<typename Scheme::GridType>&& Solver<Scheme>::solve() &&
{
    this->calculation_implementation();
    return std::move(this->temp);
}

template<typename Scheme>
void Solver<Scheme>::solve(const without_output_t& param) &&
{
    this->calculation_implementation();
}

template<typename Scheme>
void Solver<Scheme>::calculation_implementation()
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
    if (!this->geometry                 .has_value())   throw std::runtime_error("Geometric dimensions are not defined");
    if (!this->prop                     .has_value())   throw std::runtime_error("There are no thermophysical properties of the calculated body");
    if (!this->boundary_conditions      .has_value())   throw std::runtime_error("There are no boundary conditions parameters");
    if (!this->time_end                 .has_value())   throw std::runtime_error("The end time of the calculation is not specified.");
    if (!this->time_step                .has_value())   throw std::runtime_error("Time step not specified");

    for (std::size_t time_index = 0, end_time_index = *this->time_end / *this->time_step; time_index != end_time_index; ++time_index)
    {
        this->evaluate();
    }
}

template class Solver<ExplicitScheme<ProblemDimension::_2D>>;

template class Solver<ImplicitScheme<ProblemDimension::_2D>>;
template class Solver<ImplicitScheme<ProblemDimension::_1D>>;

