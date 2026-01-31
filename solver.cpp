#include "solver.h"


template class Solver<ExplicitScheme>;


template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_prop(const Properties& prop) &&
{
    this->prop = std::make_shared<Properties>(prop);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_grid(const RegularGrid& grid) &&
{
    this->temp = Temperature(grid);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_boundary_conditions(const Boundary& boundary_conds) && 
{
    this->boundary_conditions = std::make_shared<Boundary>(boundary_conds);
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_end(double time_end) &&
{
    this->time_end = time_end;
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_time_partitions(std::size_t time_partitions) &&
{
    this->time_partitions = time_partitions;
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_Q_extend(double Q_extend) &&
{
    this->Q_extend = Q_extend;
    return std::move(*this);
}

template<typename Scheme>
Solver<Scheme>&& Solver<Scheme>::set_initial_values(double init_temp) &&
{
    this->temp.fill(init_temp);
    return std::move(*this);
}

template<typename Scheme>
Temperature Solver<Scheme>::solve() &&
{
    this->x_step = this->prop->length / (this->temp.size_x() - 1);
    this->y_step = this->prop->width / (this->temp.size_y() - 1);

    this->time_step = this->time_end / this->time_partitions;

    this->new_temp = this->temp;

    for (std::size_t time_index = 0, end_time_index = this->time_partitions; time_index != end_time_index; ++time_index)
    {
        this->evaluate();
    }
    return std::move(this->temp);
}