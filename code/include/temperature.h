#ifndef TEMPERATURE
#define TEMPERATURE

#include "grid.h"
#include "properties.h"

#include <cmath>
#include <iomanip>
#include <iostream>


template<class GridType>
class Temperature : public GridType
{
	std::underlying_type_t<ConvertFlags> flag_ = is_Kelvin;
public:
	Temperature() = default;

	Temperature(const GridType& grid);

	void fill(double values);
	void show(std::size_t precision = 0);

	Temperature& to_Kelvin_deg();
	Temperature& to_Celsius_deg();
};

template<class GridType>
Temperature<GridType>::Temperature(const GridType& grid)
    : GridType(grid)
{}

template<class GridType>
Temperature(const GridType& grid) -> Temperature<GridType>;


template<class GridType>
void Temperature<GridType>::fill(double values)
{
    std::fill(this->begin(), this->end(), values);
}

template<class GridType>
void Temperature<GridType>::show(std::size_t precision)
{
    auto number                       = *std::max_element(this->begin(), this->end(), [](double first, double second)
                                                                                        {
                                                                                            return std::abs(first) < std::abs(second);
                                                                                        });
    auto number_without_fract_part    = std::floor(number);
    auto precision_and_comma          = precision + 1;
    auto width                        = count_digit(number_without_fract_part) + precision_and_comma;

    for (std::size_t y_index = 0, y_index_end = this->size_y(); y_index != y_index_end; ++y_index)
    {
        for (std::size_t x_index = 0, x_index_end = this->size_x(); x_index != x_index_end; ++x_index)
        {
            std::cout   << std::setw(width) << std::fixed << std::setprecision(precision) << std::left
                        << this->view_(y_index, x_index) << "  ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

template<class GridType>
Temperature<GridType>& Temperature<GridType>::to_Kelvin_deg()
{
    if (flag_ & ConvertFlags::is_Kelvin) return *this;

    std::for_each(this->begin(), this->end(), [](double& temp)
                                                {
                                                    temp += Properties::deg_Kelvin_is;
                                                });

    flag_ &= ~ConvertFlags::is_Celsius;
    flag_ |= ConvertFlags::is_Kelvin;

    return *this;
}

template<class GridType>
Temperature<GridType>& Temperature<GridType>::to_Celsius_deg()
{
    if (flag_ & ConvertFlags::is_Celsius) return *this;

    std::for_each(this->begin(), this->end(), [](double& temp)
                                                {
                                                    temp -= Properties::deg_Kelvin_is;
                                                });

    flag_ &= ~ConvertFlags::is_Kelvin;
    flag_ |= ConvertFlags::is_Celsius;

    return *this;
}

#endif
