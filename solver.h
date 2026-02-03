#ifndef SOLVER_H
#define SOLVER_H

#include <memory>

#include "scheme.h"


template<typename Scheme>
class Solver : public Scheme
{
public:
	Solver&& set_prop(const Properties& prop) &&;

	Solver&& set_grid(const RegularGrid& grid) &&;

	Solver&& set_boundary_conditions(const Boundary& boundary_conds) &&;

	Solver&& set_time_end(double time_end) &&;

	Solver&& set_time_step(std::size_t time_step) &&;

	Solver&& set_Q_extend(double Q_extend) &&;

	Solver&& set_initial_values(double init_temp) &&;


	Temperature solve() &&;
};


#endif // !SOLVER_H

