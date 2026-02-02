#ifndef SCHEME_H
#define SCHEME_H

#include <memory>
#include <optional>

#include "temperature.h"
#include "properties.h"
#include "boundary.h"


class ExplicitScheme
{
protected:
	Temperature temp;
	Temperature new_temp;

	std::shared_ptr<Properties> prop;

	std::shared_ptr<Boundary> boundary_conditions;

	std::size_t time_partitions;
	double time_end;
	double time_step;

	double x_step;
	double y_step;

	double Q_extend = 0;
	double CONST;

protected:
	void evaluate();
private:
	void inner_explicit_scheme(std::size_t i, std::size_t j);

	void lower_left_explicit_scheme(std::size_t i, std::size_t j);
	void upper_left_explicit_scheme(std::size_t i, std::size_t j);
	void upper_right_explicit_scheme(std::size_t i, std::size_t j);
	void lower_right_explicit_scheme(std::size_t i, std::size_t j);

	void left_explicit_scheme(std::size_t i, std::size_t j);
	void right_explicit_scheme(std::size_t i, std::size_t j);
	void up_explicit_scheme(std::size_t i, std::size_t j);
	void down_explicit_scheme(std::size_t i, std::size_t j);

	const bool check_criterion() const;
};

#endif // !SCHEME_H
