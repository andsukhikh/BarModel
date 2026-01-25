#include "grid.h"

#include <iostream>
#include <iomanip>
#include <string>


ProxyArray::ProxyArray(std::size_t total_number_of_point_x_axis)
    : x_axis(total_number_of_point_x_axis)
{}

double& ProxyArray::along_x(std::size_t index)
{
    return x_axis.at(index);
}

decltype(auto) ProxyArray::begin()          { return x_axis.begin(); }
decltype(auto) ProxyArray::end()            { return x_axis.end(); }
decltype(auto) ProxyArray::cbegin() const   { return x_axis.cbegin(); }
decltype(auto) ProxyArray::cend() const     { return x_axis.cend(); }

const std::size_t ProxyArray::size() const
{
    return x_axis.size();
}

RegularGrid::RegularGrid(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
    : y_axis(total_number_of_point_y_axis, ProxyArray(total_number_of_point_x_axis))
{}

ProxyArray& RegularGrid::along_y(std::size_t index)
{
    return y_axis.at(index);
}

void RegularGrid::show()
{
    auto max_number_digit_y = count_digit(y_axis.end() - y_axis.begin());

    std::cout << std::setw(max_number_digit_y) << " " << "X" << "\n"
        << std::setw(max_number_digit_y) << " " << "^" << "\n";

    for (auto iterator_y = y_axis.rbegin(); iterator_y != y_axis.rend(); ++iterator_y)
    {
        auto index = y_axis.crend() - iterator_y;
        std::cout << std::setw(max_number_digit_y) << index << "|";

        for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
        {
            auto index_x = iterator_x - iterator_y->cbegin() + 1;
            std::cout << index_x;
            if (iterator_x != iterator_y->cend() - 1) std::cout << "--";
        }

        auto y_axis_indent = max_number_digit_y + 1;
        std::cout << "\n";

        if (iterator_y != y_axis.crend() - 1)
        {
            std::cout << std::setw(y_axis_indent) << "|";
            for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
            {
                auto index_x = iterator_x - iterator_y->cbegin() + 1;
                auto digit_number = count_digit(index_x);
                auto middle_pos = digit_number / 2;
                auto indent = digit_number + middle_pos;
                std::cout << "|" << " " << std::string(indent, ' ');
            }
            std::cout << "\n";
        }
    }
}

const std::size_t RegularGrid::size_x() const
{
    return y_axis.at(0).size();
}

const std::size_t RegularGrid::size_y() const
{
    return y_axis.size();
}

const std::size_t RegularGrid::count_digit(std::size_t number) const
{
    return std::to_string(number).size();
}
