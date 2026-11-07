#ifndef SCHEME_H
#define SCHEME_H

#include <memory>
#include <optional>
#include <type_traits>
#include <array>
#include <limits>
#include <experimental/mdspan>

#include "enums.h"
#include "temperature.h"
#include "properties.h"
#include "boundary.h"
#include "geometry.h"
#include "grid.h"
#include "matrix.h"


template<Dimension Dim, class ConcreteScheme>
class SchemeBase
{
public:
	static constexpr auto Rank = Dim; 
protected:
	using GridType = RegularGrid<Dim>;

	Temperature<GridType> temp;

	std::optional<GridType> grid								= {};
	std::optional<Properties> prop								= {};
	std::optional<Boundary> boundary_conditions					= {};
	std::optional<double> time_end								= {};
	std::optional<double> temp_init								= {};
	std::optional<double> time_step								= {};
	std::optional<double> Q_extend								= {};

	// std::array<double, Rank> step    							= {};
public:
	void evaluate()
	{
		static_cast<ConcreteScheme*>(this)->evaluate_impl();
	};
};


template<Dimension Dim>
class ExplicitScheme : public SchemeBase<Dim, ExplicitScheme<Dim>>
{};

template<Dimension Dim>
class ImplicitScheme : public SchemeBase<Dim, ImplicitScheme<Dim>>
{};


template<>
class ExplicitScheme<ProblemDimension::_2D> : public SchemeBase<ProblemDimension::_2D, 
																ExplicitScheme<ProblemDimension::_2D>
																>
{
public:
	using GeomType = Geometry<Length, Width>;
protected:
	Temperature<GridType> new_temp;

	std::underlying_type_t<CopyFlags> flag_ = not_copied;
	std::optional<GeomType> geometry						= {};


	double x_step = std::numeric_limits<double>::max();
	double y_step = std::numeric_limits<double>::max();
	
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
	void init();
	void init_geom();
public:
	void evaluate_impl();
};


template<>
class ImplicitScheme<ProblemDimension::_2D> : public SchemeBase<ProblemDimension::_2D, 
																ImplicitScheme<ProblemDimension::_2D>
																>
{
public:
	using GeomType = Geometry<Length, Width>;
protected:
	std::optional<GeomType> geometry							= {};
public:
	void evaluate_impl();
};


template<>
class ImplicitScheme<ProblemDimension::_1D> : public SchemeBase<ProblemDimension::_1D, 
																ImplicitScheme<ProblemDimension::_1D>
																>
{
public:
	using GeomType = Geometry<Length>;
protected:
	std::optional<GeomType> geometry							= {};

	double step = std::numeric_limits<double>::max();
private:
	void init_step();
public:
	void evaluate_impl();
};



#endif
