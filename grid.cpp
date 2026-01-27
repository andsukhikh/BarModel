#include "grid.h"

#include <iostream>
#include <iomanip>
#include <string>


ProxyArray::ProxyArray(std::size_t total_number_of_point_x_axis)
    : x_axis_(total_number_of_point_x_axis)
{}

double& ProxyArray::along_x(std::size_t index)
{
    return x_axis_.at(index);
}

std::vector<double>::iterator ProxyArray::begin()          { return x_axis_.begin(); }
std::vector<double>::iterator ProxyArray::end()            { return x_axis_.end(); }
std::vector<double>::const_iterator ProxyArray::cbegin() const   { return x_axis_.cbegin(); }
std::vector<double>::const_iterator ProxyArray::cend() const     { return x_axis_.cend(); }

const std::size_t ProxyArray::size() const
{
    return x_axis_.size();
}



//RegularGrid::RegularGrid(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
//    : y_axis_(total_number_of_point_y_axis, ProxyArray(total_number_of_point_x_axis))
//{}
//
//ProxyArray& RegularGrid::along_y(std::size_t index)
//{
//    return y_axis_.at(index);
//}
//
//
//void RegularGrid::show()
//{
//    auto max_number_digit_y = count_digit(y_axis_.end() - y_axis_.begin());
//
//    std::cout << std::setw(max_number_digit_y) << " " << "X" << "\n"
//        << std::setw(max_number_digit_y) << " " << "^" << "\n";
//
//    for (auto iterator_y = y_axis_.rbegin(); iterator_y != y_axis_.rend(); ++iterator_y)
//    {
//        auto index = y_axis_.crend() - iterator_y;
//        std::cout << std::setw(max_number_digit_y) << index << "|";
//
//        for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
//        {
//            auto index_x = iterator_x - iterator_y->cbegin() + 1;
//            std::cout << index_x;
//            if (iterator_x != iterator_y->cend() - 1) std::cout << "--";
//        }
//
//        auto y_axis_indent = max_number_digit_y + 1;
//        std::cout << "\n";
//
//        if (iterator_y != y_axis_.crend() - 1)
//        {
//            std::cout << std::setw(y_axis_indent) << "|";
//            for (auto iterator_x = iterator_y->cbegin(); iterator_x != iterator_y->cend(); ++iterator_x)
//            {
//                auto index_x = iterator_x - iterator_y->cbegin() + 1;
//                auto digit_number = count_digit(index_x);
//                auto middle_pos = digit_number / 2;
//                auto indent = digit_number + middle_pos;
//                std::cout << "|" << " " << std::string(indent, ' ');
//            }
//            std::cout << "\n";
//        }
//    }
//}
//
//const std::size_t RegularGrid::size_x() const
//{
//    return y_axis_.at(0).size();
//}
//
//const std::size_t RegularGrid::size_y() const
//{
//    return y_axis_.size();
//}

const std::size_t RegularGrid::count_digit(std::size_t number) const
{
    return std::to_string(number).size();
}

const bool RegularGrid::is_lower_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && y_index == 0;
}

const bool RegularGrid::is_upper_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && y_index == size_y() - 1;
}

const bool RegularGrid::is_upper_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && y_index == size_y() - 1;
}

const bool RegularGrid::is_lower_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && y_index == 0;
}

const bool RegularGrid::is_left(std::size_t x_index, std::size_t y_index) const
{
    return x_index == 0 && !is_lower_left(x_index, y_index) && !is_upper_left(x_index, y_index);
}

const bool RegularGrid::is_right(std::size_t x_index, std::size_t y_index) const
{
    return x_index == size_x() - 1 && !is_lower_right(x_index, y_index) && !is_upper_right(x_index, y_index);
}

const bool RegularGrid::is_up(std::size_t x_index, std::size_t y_index) const
{
    return y_index == size_y() - 1 && !is_upper_left(x_index, y_index) && !is_upper_right(x_index, y_index);
}

const bool RegularGrid::is_down(std::size_t x_index, std::size_t y_index) const
{
    return y_index == 0 && !is_lower_left(x_index, y_index) && !is_lower_right(x_index, y_index);
}


RegularGrid::RegularGrid(std::size_t total_number_of_point_x_axis, std::size_t total_number_of_point_y_axis)
    : grid_(total_number_of_point_y_axis, std::vector<double>(total_number_of_point_x_axis))
{}

double& RegularGrid::operator() (std::size_t x_index, std::size_t y_index)
{
    return grid_.at(y_index).at(x_index);
}


void RegularGrid::show()
{
    auto max_number_digit_y = count_digit(grid_.end() - grid_.begin());

    std::cout << std::setw(max_number_digit_y) << " " << "X" << "\n"
        << std::setw(max_number_digit_y) << " " << "^" << "\n";

    for (auto iterator_y = grid_.rbegin(), iterator_y_end = grid_.rend(); iterator_y != iterator_y_end; ++iterator_y)
    {
        auto index = grid_.crend() - iterator_y;
        std::cout << std::setw(max_number_digit_y) << index << "|";

        for (auto iterator_x = iterator_y->cbegin(), iterator_x_end = iterator_y->cend(); iterator_x != iterator_x_end; ++iterator_x)
        {
            auto index_x = iterator_x - iterator_y->cbegin() + 1;
            std::cout << index_x;

            if (iterator_x != iterator_y->cend() - 1) std::cout << "--";
        }

        auto y_axis_indent = max_number_digit_y + 1;
        std::cout << "\n";

        if (iterator_y != grid_.crend() - 1)
        {
            std::cout << std::setw(y_axis_indent) << "|";
            for (auto iterator_x = iterator_y->cbegin(), iterator_x_end = iterator_y->cend(); iterator_x != iterator_x_end; ++iterator_x)
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
    return grid_.at(0).size();
}

const std::size_t RegularGrid::size_y() const
{
    return grid_.size();
}