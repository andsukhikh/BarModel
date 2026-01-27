#include <iostream>
#include <iomanip>

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

void Temperature::show()
{
    for (auto iterator_y = grid_.crbegin(), iterator_y_end = grid_.crend(); iterator_y != iterator_y_end; ++iterator_y)
    {
        for (auto iterator_x = iterator_y->cbegin(), iterator_x_end = iterator_y->cend(); iterator_x != iterator_x_end; ++iterator_x)
        {
            std::cout.setf(std::ios::fixed);
            std::cout.precision(1);

            auto x_index = iterator_x - iterator_y->cbegin();
            auto y_index = iterator_y_end - 1 - iterator_y;

            std::cout << std::setw(5) << grid_[y_index][x_index] << "  ";
        }
        std::cout << "\n";
    }
}
