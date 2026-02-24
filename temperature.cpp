#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "temperature.h"
#include "properties.h"

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
    auto number                       = *std::max_element(grid_.begin(), grid_.end(), [](double first, double second)
                                                                                        {
                                                                                            return std::abs(first) < std::abs(second);
                                                                                        });
    auto number_without_fract_part    = std::floor(number);
    auto precision_and_comma          = precision + 1;
    auto width                        = RegularGrid::count_digit(number_without_fract_part) + precision_and_comma;

    for (auto&& row_offset : row_number_)
    {
        for (std::size_t x_index = 0, x_index_end = size_x(); x_index != x_index_end; ++x_index)
        {
            std::cout   << std::setw(width) << std::fixed << std::setprecision(precision) << std::left
                        << grid_[row_offset + x_index] << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Temperature& Temperature::to_Kelvin_deg()
{
    if (flag_ & ConvertFlags::is_Kelvin) return *this;

    std::for_each(grid_.begin(), grid_.end(), [](double& temp)
                                                {
                                                    temp += Properties::deg_Kelvin_is;
                                                });

    flag_ &= ~ConvertFlags::is_Celsius;
    flag_ |= ConvertFlags::is_Kelvin;

    return *this;
}

Temperature& Temperature::to_Celsius_deg()
{
    if (flag_ & ConvertFlags::is_Celsius) return *this;

    std::for_each(grid_.begin(), grid_.end(), [](double& temp)
                                                {
                                                    temp -= Properties::deg_Kelvin_is;
                                                });

    flag_ &= ~ConvertFlags::is_Kelvin;
    flag_ |= ConvertFlags::is_Celsius;

    return *this;
}