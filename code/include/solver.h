#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include "boundary.h"
#include "properties.h"
#include "temperature.h"
#include "geometry.h"

struct without_output_t
{
	explicit without_output_t() = default;
};

inline constexpr without_output_t without_output {};


template<typename Scheme>
class Solver : public Scheme
{
public:
	Solver&& set_prop(const Properties& prop) &&;

	Solver&& set_grid(const RegularGrid<Scheme::Rank>& grid) &&;

	Solver&& set_geometry(const Scheme::GeomType& geom) &&;

	Solver&& set_boundary_conditions(const Boundary& boundary_conds) &&;

	Solver&& set_time_end(double time_end) &&;

	Solver&& set_time_step(std::size_t time_step) &&;

	Solver&& set_Q_extend(double Q_extend) &&;

	Solver&& set_initial_values(double temp_init) &&;

	Temperature<typename Scheme::GridType>&& solve() &&;
	void solve(const without_output_t&) &&;

public:
	void calculation_implementation();
};

#endif

