#ifndef SCHEME_H
#define SCHEME_H

#include <memory>
#include <optional>
#include <type_traits>

#include "temperature.h"
#include "properties.h"
#include "boundary.h"


enum CopyFlags : char
{
	no_copied		= 0b0000'0000,
	copied			= 0b0000'0001 
};


class ExplicitScheme
{
	std::underlying_type_t<CopyFlags> flag_ = no_copied;
protected:
	Temperature temp;
	Temperature new_temp;

	std::optional<RegularGrid> grid					= {};
	std::optional<Properties> prop					= {};
	std::optional<Boundary> boundary_conditions		= {};
	std::optional<double> temp_init					= {};
	std::optional<double> time_end					= {};
	std::optional<double> time_step					= {};
	std::optional<double> Q_extend					= {};

	double x_step;
	double y_step;
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

	void init_new_temp();
};


class ImplicitScheme
{
protected:
	Temperature temp;

	std::optional<RegularGrid> grid					= {};
	std::optional<Properties> prop					= {};
	std::optional<Boundary> boundary_conditions		= {};
	std::optional<double> temp_init					= {};
	std::optional<double> time_end					= {};
	std::optional<double> time_step					= {};
	std::optional<double> Q_extend					= {};

	double x_step;
	double y_step;
protected:
	void evaluate();
};


#endif // !SCHEME_H
