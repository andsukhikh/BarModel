#include <iostream>
#include <iomanip>
#include <algorithm>

#include "temperature.h"

Temperature::Temperature(std::size_t x_partitions_number, std::size_t y_partitions_number)
    : RegularGrid(x_partitions_number, y_partitions_number)
{}

Temperature::Temperature(const RegularGrid& grid)
    : RegularGrid(grid)
{}

void Temperature::fill(double values)
{
    std::fill(grid_.begin(), grid_.end(), values);
}

void Temperature::show(std::size_t precision)
{
    auto number                       = std::max_element(grid_.begin(), grid_.end());
    auto number_without_fract_part    = static_cast<std::size_t>(*number);
    auto width = RegularGrid::count_digit(number_without_fract_part) + precision + 1;

    for (auto&& row_offset : row_number_)
    {
        for (std::size_t x_index = 0, x_index_end = size_x(); x_index != x_index_end; ++x_index)
        {
            std::cout   << std::setw(width) << std::fixed << std::setprecision(precision)
                        << grid_[row_offset + x_index] << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Temperature::to_Kelvin_deg()
{
    if (flag_ & ConvertFlags::is_Kelvin) return;

    std::for_each(grid_.begin(), grid_.end(), [](double& temp)
        {
            temp += 273.15;
        });

    flag_ = is_Kelvin;
}

void Temperature::to_Celsius_deg()
{
    if (flag_ & ConvertFlags::is_Celsium) return;

    std::for_each(grid_.begin(), grid_.end(), [](double& temp)
        {
            temp -= 273.15;
        });

    flag_ = is_Celsium;
}
