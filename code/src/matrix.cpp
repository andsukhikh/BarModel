#include "matrix.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>


Ordinary::Ordinary(std::size_t row_size, std::size_t column_size)
	: row_size_(row_size)
	, column_size_(column_size)
	, matrix_(row_size * column_size)
	, index_of_first_row_elem_(row_size)
{
	std::size_t start_of_new_row_index = 0;
	std::for_each(index_of_first_row_elem_.begin(), index_of_first_row_elem_.end(), [&](std::size_t& index)
		{
			index = start_of_new_row_index;
			start_of_new_row_index += column_size;
		});
}

double& Ordinary::operator() (std::size_t row, std::size_t column)
{
	if (row < row_size_ && column < column_size_)
	{
		auto row_offset = index_of_first_row_elem_.at(row);
		return matrix_.at(row_offset + column);
	}
	throw std::out_of_range("Row or column index out of bounds");
}


Tridiagonal& Tridiagonal::set_solution_method(UniquePtrSolveMethod&& solve_method)
{
	solve_method_ = std::move(solve_method);
	return *this;
}

std::vector<double> ShuttleMethod::solve(std::vector<double>& free_column, Tridiagonal& matrix)
{
	auto has_zero = std::any_of(matrix.main_diagonal_.cbegin(), matrix.main_diagonal_.cend(),
		[&matrix](const double& elem) 
		{
			return elem == matrix.zero_elem_;
		});

	if (has_zero) throw std::range_error("The main diagonal matrix contains zero elements");

	auto& main_diag		= matrix.main_diagonal_;
	auto& upper			= matrix.upper_diagonal_;
	auto& down			= matrix.down_diagonal_;

	std::size_t dim = matrix.dimension_;

	for (std::size_t index = 1; index < dim; ++index) {
		double coef = down[index] / main_diag[index - 1];
		main_diag[index] -= coef * upper[index - 1];
		free_column[index] -= coef * free_column[index - 1];
	}

	std::vector<double> solve_vector(dim);
	solve_vector[dim - 1] = free_column[dim - 1] / main_diag[dim - 1];

	for (int index = dim - 2; index >= 0; --index) {
		solve_vector[index] = (free_column[index] - upper[index] * solve_vector[index + 1]) / main_diag[index];
	}

	return solve_vector;
}


Tridiagonal::Tridiagonal(std::size_t dimension)
	: dimension_(dimension)
	, zero_elem_{}
	, main_diagonal_(dimension)
	, upper_diagonal_(dimension)
	, down_diagonal_(dimension)
	, other_elems_{}
	, solve_method_(nullptr)
{}

const double& Tridiagonal::operator() (std::size_t row, std::size_t column) const
{
	if (row < dimension_ && column < dimension_)
	{
		if (row == column)		return main_diagonal_[row];
		if (row == column - 1)	return upper_diagonal_[column - 1];
		if (row == column + 1)	return down_diagonal_[column + 1];

		return other_elems_;
	}
	throw std::out_of_range("Row or column index out of bounds");
}

double& Tridiagonal::operator() (std::size_t row, std::size_t column)
{
	if (row < dimension_ && column < dimension_)
	{
		if (row == column)		return main_diagonal_[row];
		if (row == column - 1)	return upper_diagonal_[column - 1];
		if (row == column + 1)	return down_diagonal_[column + 1];

		return other_elems_;
	}
	throw std::out_of_range("Row or column index out of bounds");
}



std::vector<double> Tridiagonal::solve(std::vector<double> free_column)
{
	return solve_method_->solve(free_column, *this);
}

std::size_t Tridiagonal::dimension() const
{
	return dimension_;
}


std::ostream& operator<< (std::ostream& output_stream, const Tridiagonal& this_matrix)
{
	for (std::size_t row = 0, dim = this_matrix.dimension(); row != dim; ++row)
	{
		for (std::size_t column = 0; column != dim; ++column)
		{
			output_stream << this_matrix(row, column) << " ";
		}

		output_stream << "\n";
	}

	return output_stream;
}

