#include "temperature.h"

Temperature::Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
    : grid_(std::make_shared<RegularGrid>(total_number_of_point_x_axis, total_number_of_point_y_axis))
{}

//Temperature::Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
//    : RegularGrid(total_number_of_point_x_axis, total_number_of_point_y_axis)
//{}

Temperature::Temperature(const RegularGrid& grid)
    : grid_(std::make_shared<RegularGrid>(grid))
{}

//Temperature::Temperature(const RegularGrid& grid)
//    : RegularGrid(grid.size_x(), grid.size_y())
//{}

Temperature::Temperature(const Temperature& temperature)
{
    grid_ = temperature.grid_;
}

Temperature& Temperature::operator=(const Temperature& temperature)
{
    grid_ = temperature.grid_;
    return *this;
}

ProxyArray& Temperature::along_y(std::size_t index)
{
    return grid_->along_y(index);
}

const std::size_t Temperature::size_x() const
{
    return grid_->size_x();
}

const std::size_t Temperature::size_y() const
{
    return grid_->size_y();
}