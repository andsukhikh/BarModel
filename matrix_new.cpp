#include "matrix_new.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>


template <typename DataType>
Ordinary<DataType>::Ordinary(std::size_t row_size, std::size_t column_size)
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

template <typename DataType>
DataType& Ordinary<DataType>::operator() (std::size_t row, std::size_t column)
{
	if (row < row_size_ && column < column_size_)
	{
		auto row_offset = index_of_first_row_elem_.at(row);
		return matrix_.at(row_offset + column);
	}
	throw std::out_of_range("Row or column index out of bounds");
}


template <typename DataType>
Tridiagonal<DataType>& Tridiagonal<DataType>::set_solve_method(UniquePtrSolveMethod&& solve_method)
{
	solve_method_ = std::move(solve_method);

	return *this;
}


template <typename DataType>
std::vector<DataType> ShuttleMethod<DataType>::solve(std::vector<DataType>& free_column, Tridiagonal<DataType>& matrix)
{
	bool hasZero = std::any_of(matrix.main_diagonal_.cbegin(), matrix.main_diagonal_.cend(),
		[&matrix](const DataType& elem) 
		{
			return elem == matrix.zero_elem_;
		});

	if (hasZero) throw std::range_error("The main diagonal matrix contains zero elements");

	std::vector<DataType>& main_diag = matrix.main_diagonal_;
	std::vector<DataType>& upper = matrix.upper_diagonal_;
	std::vector<DataType>& down = matrix.down_diagonal_;
	std::size_t dim = matrix.dimension_;

	for (std::size_t i = 1; i < dim; ++i) {
		DataType coef = down[i] / main_diag[i - 1];
		main_diag[i] -= coef * upper[i - 1];
		free_column[i] -= coef * free_column[i - 1];
	}

	std::vector<DataType> solve_vector(dim);
	solve_vector[dim - 1] = free_column[dim - 1] / main_diag[dim - 1];

	for (int i = dim - 2; i >= 0; --i) {
		solve_vector[i] = (free_column[i] - upper[i] * solve_vector[i + 1]) / main_diag[i];
	}

	return solve_vector;

	//return std::vector<DataType>(3);
}


template <typename DataType>
Tridiagonal<DataType>::Tridiagonal(std::size_t dimension)
	: dimension_(dimension)
	, zero_elem_{}
	, main_diagonal_(dimension)
	, upper_diagonal_(dimension)
	, down_diagonal_(dimension)
	, other_elems_{}
	, solve_method_(nullptr)
{}

template <typename DataType>
const DataType& Tridiagonal<DataType>::operator() (std::size_t row, std::size_t column) const
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

template <typename DataType>
DataType& Tridiagonal<DataType>::operator() (std::size_t row, std::size_t column)
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



template<typename DataType>
std::vector<DataType> Tridiagonal<DataType>::solve(std::vector<DataType> free_column)
{
//	auto checking_nonzero_elem = std::any_of(main_diagonal_.cbegin(), main_diagonal_.cend(),
//		[this](const DataType& elem)
//		{
//			return elem != zero_elem_;
//		});
//
//	if (!checking_nonzero_elem) throw std::range_error("The main diagonal matrix contains zero elements");
//
//	main_diagonal_[0] = main_diagonal_[0];
//	free_column[0] = free_column[0];
//
//	for (std::size_t index = 1; index != dimension_; ++index)
//	{
//		auto coef = down_diagonal_[index] / main_diagonal_[index - 1];
//		main_diagonal_[index] -= coef * upper_diagonal_[index - 1];
//		free_column[index] -= coef * free_column[index - 1];
//	}
//
//	std::vector<DataType> solve_vector(dimension_);
//
//	std::fill(down_diagonal_.begin(), down_diagonal_.end(), zero_elem_);
//	
//	*solve_vector.rbegin() = *free_column.rbegin() / *main_diagonal_.rbegin();
//	
//	for (int index = dimension_ - 2; index >= 0; --index)
//	{
//		auto first_coef	 = free_column[index] / main_diagonal_[index];
//		auto second_coef = upper_diagonal_[index] / main_diagonal_[index];
//	
//		solve_vector[index] = first_coef - second_coef * solve_vector[index + 1];
//	}
//	
//	return solve_vector;

	return solve_method_->solve(free_column, *this);
}

template<typename DataType>
std::size_t Tridiagonal<DataType>::dimension() const
{
	return dimension_;
}

template <typename DataType = double>
std::ostream& operator<< (std::ostream& output_stream, const Tridiagonal<DataType>& this_matrix)
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

