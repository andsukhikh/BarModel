#include "temperature.h"

Temperature::Temperature(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
    : RegularGrid(total_number_of_point_x_axis, total_number_of_point_y_axis)
{}

Temperature::Temperature(const RegularGrid& grid)
    : RegularGrid(grid)
{}


void Temperature::fill(double values)
{
    //for (auto&& y_line : y_axis_)
    //{
    //    std::fill(y_line.begin(), y_line.end(), values);
    //}

    for (auto&& y_line : grid_)
    {
        std::fill(y_line.begin(), y_line.end(), values);
    }
}