#include <vector>
#include <iostream>
#include <memory>
#include <format>
#include <string>
#include <iomanip>
#include <exception>
#include <optional>

#include "solver.h"


Solver& Solver::set_scheme(std::shared_ptr<IScheme> scheme)
{
    scheme_ = scheme;
    return *this;
}

Solver& Solver::set_prop(const Properties& prop)
{
    scheme_->prop = std::make_shared<Properties>(prop);
    return *this;
}

Solver& Solver::set_grid(const RegularGrid& grid)
{
    scheme_->temp = std::make_shared<Temperature>(grid);
    return *this;
}

Solver& Solver::set_boundary_conditions(const Boundary& boundary_conds)
{
    scheme_->boundary_conditions = std::make_shared<Boundary>(boundary_conds);
    return *this;
}

Solver& Solver::set_time_end(double time_end)
{
    scheme_->time_end = time_end;
    return *this;
}

Solver& Solver::set_time_partitions(std::size_t time_partitions)
{
    scheme_->time_partitions = time_partitions;
    return *this;
}

Solver& Solver::set_Q_extend(double Q_extend)
{
    scheme_->Q_extend = Q_extend;
    return *this;
}

std::shared_ptr<Temperature> Solver::solve()
{
    for (std::size_t time_index = 0, end_time_index = scheme_->time_partitions; time_index != end_time_index; ++time_index)
    {
        scheme_->evaluate();
    }
    return scheme_->temp;
}
