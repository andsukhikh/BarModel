#include <memory>

#include "grid.h"

const bool RegularGrid<ProblemDimension::_2D>::is_lower_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && y_index == 0;
}

const bool RegularGrid<ProblemDimension::_2D>::is_upper_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && y_index == size_y() - 1;
}

const bool RegularGrid<ProblemDimension::_2D>::is_upper_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && y_index == size_y() - 1;
}

const bool RegularGrid<ProblemDimension::_2D>::is_lower_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && y_index == 0;
}

const bool RegularGrid<ProblemDimension::_2D>::is_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && !is_lower_left(x_index, y_index) && !is_upper_left(x_index, y_index);
}

const bool RegularGrid<ProblemDimension::_2D>::is_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && !is_lower_right(x_index, y_index) && !is_upper_right(x_index, y_index);
}

const bool RegularGrid<ProblemDimension::_2D>::is_up(std::size_t x_index, std::size_t y_index) const
{
    return y_index == size_y() - 1 && !is_upper_left(x_index, y_index) && !is_upper_right(x_index, y_index);
}

const bool RegularGrid<ProblemDimension::_2D>::is_down(std::size_t x_index, std::size_t y_index) const
{
    return y_index == 0 && !is_lower_left(x_index, y_index) && !is_lower_right(x_index, y_index);
}

RegularGrid<ProblemDimension::_2D>::RegularGrid(std::size_t x_size, std::size_t y_size)
    : buffer_(y_size * x_size)
    , view_(buffer_.data(), y_size, x_size)
{}

RegularGrid<ProblemDimension::_2D>::RegularGrid(const RegularGrid& other)
    : buffer_(other.buffer_)
    , view_(buffer_.data(), other.size_y(), other.size_x())
{}

RegularGrid<ProblemDimension::_2D>& RegularGrid<ProblemDimension::_2D>::operator=(const RegularGrid& other) 
{
    buffer_ = other.buffer_;
    view_   = Grid(buffer_.data(), other.size_y(), other.size_x());
    return *this;
}

double& RegularGrid<ProblemDimension::_2D>::operator() (std::size_t x_index, std::size_t y_index)
{
    const auto y_index_offset = size_y() - 1 - y_index;
    return view_(y_index_offset, x_index);
}

const double& RegularGrid<ProblemDimension::_2D>::operator() (std::size_t x_index, std::size_t y_index) const
{
    const auto y_index_offset = size_y() - 1 - y_index;
    return view_(y_index_offset, x_index);
}

const std::size_t RegularGrid<ProblemDimension::_2D>::size_x() const
{
    return view_.extent(Axis::X);
}

const std::size_t RegularGrid<ProblemDimension::_2D>::size_y() const
{
    return view_.extent(Axis::Y);
}

RegularGrid<ProblemDimension::_2D>::iterator RegularGrid<ProblemDimension::_2D>::begin()
{
    return view_.data_handle();
}

RegularGrid<ProblemDimension::_2D>::iterator RegularGrid<ProblemDimension::_2D>::end()
{
    return view_.data_handle() + view_.size();
}

RegularGrid<ProblemDimension::_2D>::const_iterator RegularGrid<ProblemDimension::_2D>::cbegin() const
{
    return view_.data_handle();
}

RegularGrid<ProblemDimension::_2D>::const_iterator RegularGrid<ProblemDimension::_2D>::cend() const
{
    return view_.data_handle() + view_.size();
}

RegularGrid<_1D> RegularGrid<ProblemDimension::_2D>::slice_y(std::uint8_t index)
{
    RegularGrid<_1D> result(size_y());

    auto row = stdex::submdspan(view_, index, stdex::full_extent);

    for (std::size_t x = 0; x < size_x(); ++x)
        result(x) = row[x];

    return result;
}

RegularGrid<_1D> RegularGrid<ProblemDimension::_2D>::slice_x(std::uint8_t index)
{
    RegularGrid<_1D> result(size_x());

    auto row = stdex::submdspan(view_, index, stdex::full_extent);

    for (std::size_t x = 0; x < size_x(); ++x)
        result(x) = row[x];

    return result;
}


const std::size_t count_digit(int number)
{
    return std::to_string(number).size();
}






const bool RegularGrid<ProblemDimension::_1D>::is_left(std::size_t index) const
{
    return index == 0;
}

const bool RegularGrid<ProblemDimension::_1D>::is_right(std::size_t index) const
{
    return index == size() - 1;
}


RegularGrid<ProblemDimension::_1D>::RegularGrid(std::size_t size)
    : buffer_(size)
    , view_(buffer_.data(), size)
{}

RegularGrid<ProblemDimension::_1D>::RegularGrid(const RegularGrid& other)
    : buffer_(other.buffer_)
    , view_(buffer_.data(), other.size())
{}

RegularGrid<ProblemDimension::_1D>& RegularGrid<ProblemDimension::_1D>::operator=(const RegularGrid& other) 
{
    buffer_ = other.buffer_;
    view_   = Grid(buffer_.data(), other.size());
    return *this;
}

double& RegularGrid<ProblemDimension::_1D>::operator() (std::size_t index)
{
    return view_(index);
}

const double& RegularGrid<ProblemDimension::_1D>::operator() (std::size_t index) const
{
    return view_(index);
}

const std::size_t RegularGrid<ProblemDimension::_1D>::size() const
{
    return view_.extent(Axis::X);
}


RegularGrid<ProblemDimension::_1D>::iterator RegularGrid<ProblemDimension::_1D>::begin()
{
    return view_.data_handle();
}

RegularGrid<ProblemDimension::_1D>::iterator RegularGrid<ProblemDimension::_1D>::end()
{
    return view_.data_handle() + view_.size();
}

double RegularGrid<ProblemDimension::_1D>::front()
{
    return buffer_.front();
}

double RegularGrid<ProblemDimension::_1D>::back()
{
    return buffer_.back();
}

const double RegularGrid<ProblemDimension::_1D>::cfront() const
{
    return buffer_.front();
}

const double RegularGrid<ProblemDimension::_1D>::cback() const
{
    return buffer_.back();
}

RegularGrid<ProblemDimension::_1D>::const_iterator RegularGrid<ProblemDimension::_1D>::cbegin() const
{
    return view_.data_handle();
}

RegularGrid<ProblemDimension::_1D>::const_iterator RegularGrid<ProblemDimension::_1D>::cend() const
{
    return view_.data_handle() + view_.size();
}

