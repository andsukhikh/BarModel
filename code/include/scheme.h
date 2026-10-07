#ifndef SCHEME_H
#define SCHEME_H

#include <memory>
#include <optional>
#include <type_traits>

#include "enums.h"
#include "temperature.h"
#include "properties.h"
#include "boundary.h"


template<typename ConreteScheme>
class SchemeBase
{
public:
	Temperature temp;

	std::optional<RegularGrid> grid								= {};
	std::optional<Properties> prop								= {};
	std::optional<Boundary> boundary_conditions					= {};
	std::optional<double> time_end								= {};
	std::optional<double> temp_init								= {};
	std::optional<double> time_step								= {};
	std::optional<double> Q_extend								= {};

	double x_step;
	double y_step;
public:
	void evaluate();
};

class ExplicitScheme : public SchemeBase<ExplicitScheme>
{
	std::underlying_type_t<CopyFlags> flag_ = not_copied;
protected:
	Temperature new_temp;
public:
	void evaluate_impl();
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

class ImplicitScheme : public SchemeBase<ImplicitScheme>
{
public:
	void evaluate_impl();
};


#endif
