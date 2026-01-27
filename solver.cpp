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
    scheme_->temp = Temperature(grid);
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

Solver& Solver::set_initial_values(double init_temp)
{
    scheme_->temp.fill(init_temp);
    return *this;
}

Temperature Solver::solve()
{
    scheme_->x_step = scheme_->prop->length / (scheme_->temp.size_x() - 1);
    scheme_->y_step = scheme_->prop->width / (scheme_->temp.size_y() - 1);

    scheme_->time_step = scheme_->time_end / scheme_->time_partitions;

    scheme_->new_temp = scheme_->temp;

    for (std::size_t time_index = 0, end_time_index = scheme_->time_partitions; time_index != end_time_index; ++time_index)
    {
        scheme_->evaluate();
    }
    return scheme_->temp;
}
