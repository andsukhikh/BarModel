#include "grid.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <numeric>

#include <source_location>



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

RegularGrid::RegularGrid(std::size_t x_partitions_number, std::size_t y_partitions_number)
    : grid_         (x_partitions_number * y_partitions_number)
    , row_number_   (y_partitions_number)
{
    std::size_t new_row_begin_index = 0;
    std::generate_n(row_number_.begin(), y_partitions_number, [&]()
        {
            std::size_t index = new_row_begin_index;
            new_row_begin_index += x_partitions_number;
            return index;
        });
}

double& RegularGrid::operator() (std::size_t x_index, std::size_t y_index)
{
    auto row_offset = row_number_.at(size_y() - 1 - y_index);
    return                  grid_.at(row_offset + x_index);
}


//void RegularGrid::show()
//{
//    auto max_number_digit_y = count_digit(grid_.end() - grid_.begin());
//
//    std::cout << std::setw(max_number_digit_y) << " " << "X" << "\n"
//        << std::setw(max_number_digit_y) << " " << "^" << "\n";
//
//    for (auto iterator_y = grid_.rbegin(), iterator_y_end = grid_.rend(); iterator_y != iterator_y_end; ++iterator_y)
//    {
//        auto index = grid_.crend() - iterator_y;
//        std::cout << std::setw(max_number_digit_y) << index << "|";
//
//        for (auto iterator_x = iterator_y->cbegin(), iterator_x_end = iterator_y->cend(); iterator_x != iterator_x_end; ++iterator_x)
//        {
//            auto index_x = iterator_x - iterator_y->cbegin() + 1;
//            std::cout << index_x;
//
//            if (iterator_x != iterator_y->cend() - 1) std::cout << "--";
//        }
//
//        auto y_axis_indent = max_number_digit_y + 1;
//        std::cout << "\n";
//
//        if (iterator_y != grid_.crend() - 1)
//        {
//            std::cout << std::setw(y_axis_indent) << "|";
//            for (auto iterator_x = iterator_y->cbegin(), iterator_x_end = iterator_y->cend(); iterator_x != iterator_x_end; ++iterator_x)
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

const std::size_t RegularGrid::size_x() const
{
    return grid_.size() / size_y();
}

const std::size_t RegularGrid::size_y() const
{
    return row_number_.size();
}