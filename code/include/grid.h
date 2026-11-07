#ifndef GRID_H
#define GRID_H

#include <experimental/mdspan>
#include <vector>

#include "enums.h"


//  	 X	
//  	^
//   2	| 10 - 11 - 12 - 13 - 14
//   	| |    |    |    |    |
//   1	| 5 -- 6 -- 7 -- 8 -- 9
//   	| |    |    |    |    |
//   0	| 0 -- 1 -- 2 -- 3 -- 4
//  	*--------------------> Y

using Dimension = std::underlying_type_t<ProblemDimension>;


namespace stdex = std::experimental;

template<Dimension Dim, typename TypeIndex = std::size_t>
using full_dynamic = stdex::dextents<TypeIndex, Dim>;

template<Dimension Dim, typename ElemType = double, typename ExtentElemType = std::size_t>
using GridDyn = stdex::mdspan<ElemType, full_dynamic<Dim, ExtentElemType>>;



template<Dimension Dim>
class RegularGrid;


template<>
class RegularGrid<ProblemDimension::_2D>
{
public:

    using iterator = double*;
    using const_iterator = const double*;

    using Grid = GridDyn<ProblemDimension::_2D>;

protected:
    std::vector<double> buffer_;
    Grid view_;

public:

    RegularGrid(std::size_t x_size, std::size_t y_size);
    RegularGrid(const RegularGrid& other);
    RegularGrid() = default;
    RegularGrid& operator=(const RegularGrid& other);

    double& operator()(std::size_t x_index, std::size_t y_index);

    const double& operator()(std::size_t x_index, std::size_t y_index) const;

    const std::size_t size_x() const;
    const std::size_t size_y() const;

    const bool is_lower_left(std::size_t x_index, std::size_t y_index) const;
    const bool is_upper_left(std::size_t x_index, std::size_t y_index) const;
    const bool is_upper_right(std::size_t x_index, std::size_t y_index) const;
    const bool is_lower_right(std::size_t x_index,std::size_t y_index) const;

    const bool is_left(std::size_t x_index,std::size_t y_index) const;
    const bool is_right(std::size_t x_index,std::size_t y_index) const;
    const bool is_up(std::size_t x_index,std::size_t y_index) const;
    const bool is_down(std::size_t x_index,std::size_t y_index) const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    RegularGrid<ProblemDimension::_1D> slice_y(std::uint8_t index);
    RegularGrid<ProblemDimension::_1D> slice_x(std::uint8_t index);
};

template<>
class RegularGrid<ProblemDimension::_1D>
{
public:

    using iterator = double*;
    using const_iterator = const double*;

    using Grid = GridDyn<ProblemDimension::_1D>;
protected:
    std::vector<double> buffer_;
    Grid view_;
public:

    explicit RegularGrid(std::size_t size);
    RegularGrid(const RegularGrid& other);
    RegularGrid& operator=(const RegularGrid& other);
	RegularGrid() = default;

    double& operator()(std::size_t index);
    const double& operator()(std::size_t index) const;

    const std::size_t size() const;

    const bool is_left(std::size_t index) const;
    const bool is_right(std::size_t index) const;

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    double front();
    double back();

    const double cfront() const;
    const double cback() const;
};

const std::size_t count_digit(int number);


#endif
