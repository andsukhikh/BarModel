#include "matrix.h"


template<std::size_t Dimension, typename TypeElem>
TridiagonalMatrix<Dimension, TypeElem>::TridiagonalMatrix()
	: lower_diagonal_(Dimension)
	, main_diagonal_(Dimension)
	, upper_diagonal_(Dimension)
{}


//template<std::size_t Dimension, typename TypeElem>
//std::vector<TypeElem> TridiagonalMatrix<Dimension, TypeElem>::solve(std::vector<TypeElem>& free_column, Mat& matrix)
//{
//	lower_diagonal_		= matrix.slice(LowerDiagonal<Dimension, TypeElem>());
//	main_diagonal_		= matrix.slice(MainDiagonal<Dimension, TypeElem>());
//	upper_diagonal_		= matrix.slice(UpperDiagonal<Dimension, TypeElem>());
//
//	main_diagonal_[0] = main_diagonal_[0];
//
//	for (std::size_t index = 1, index_end = Dimension; index != index_end; ++index)
//	{
//		auto coef = lower_diagonal_[index] / main_diagonal_[index - 1];
//		main_diagonal_[index] -= coef * upper_diagonal_[index - 1];
//
//		auto coeff = lower_diagonal_[index] / main_diagonal_[index - 1];
//		free_column[index] -= coeff * free_column[index - 1];
//	}
//
//	//for (std::size_t index = 1, index_end = Dimension; index != index_end; ++index)
//	//{
//	//	auto coef = lower_diagonal_[index] / main_diagonal_[index - 1];
//	//	free_column[index] -= coef * free_column[index - 1];
//	//}
//
//	std::vector<TypeElem> solve_vector(Dimension);
//
//	*solve_vector.rbegin() = *free_column.rbegin() / *main_diagonal_.rbegin();
//
//	for (int index = Dimension - 2; index >= 0; --index)
//	{
//		auto first_coef	 = free_column[index] / main_diagonal_[index];
//		auto second_coef = upper_diagonal_[index] / main_diagonal_[index];
//
//		solve_vector[index] = first_coef - second_coef * solve_vector[index + 1];
//	}
//
//	return solve_vector;
//}

template<std::size_t Dimension, typename TypeElem>
std::vector<TypeElem> TridiagonalMatrix<Dimension, TypeElem>::solve(std::vector<TypeElem>& free_column, Mat& matrix)
{

}


//------------  Begin  ---------------//
//---Matrix Template Implementation---//

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
Matrix<RowSize, ColumnSize, TypeElem>::Matrix()
	: matrix_(RowSize* ColumnSize)
	, row_first_elem_index_(RowSize)
	, solver_strategy_{}
{
	static_assert(RowSize > 0 && ColumnSize > 0, "Matrix dimensions must be positive");

	std::size_t new_row_begin_index = 0;
	std::for_each(row_first_elem_index_.begin(), row_first_elem_index_.end(), [&](std::size_t& index)
		{
			index = new_row_begin_index;
			new_row_begin_index += ColumnSize;
		});
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
TypeElem& Matrix<RowSize, ColumnSize, TypeElem>::operator() (std::size_t row, std::size_t column)
{
	if (row < RowSize && column < ColumnSize)
	{
		auto row_offset = row_first_elem_index_.at(row);
		return matrix_.at(row_offset + column);
	}
	throw std::out_of_range("Row or column index out of bounds");
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::slice(SliceStrategy<RowSize, ColumnSize, TypeElem>&& strategy) -> Vector
{
	return strategy.extract(*this);
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::slice(std::string_view diagonal_name) -> Vector
{
	static_assert(RowSize == ColumnSize, "Slice is only available for square matrices");

	Vector slice;
	slice.reserve(RowSize);

	if (diagonal_name == "main")
	{
		for (std::size_t index = 0; index != RowSize; ++index)
		{
			auto& ref = (*this)(index, index);
			slice.push_back(ref);
		}
	}

	else if (diagonal_name == "upper")
	{
		for (std::size_t index = 0; index != RowSize - 1; ++index)
		{
			auto& ref = (*this)(index, index + 1);
			slice.push_back(ref);
		}
	}

	else if (diagonal_name == "lower")
	{
		for (std::size_t index = 0; index != RowSize - 1; ++index)
		{
			auto& ref = (*this)(index + 1, index);
			slice.push_back(ref);
		}
	}

	else throw std::invalid_argument("There is no slice mode:" + std::string(diagonal_name));

	return slice;
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::row(std::size_t row_number) -> Vector
{
	Vector row;
	row.reserve(RowSize);

	for (std::size_t index = 0; index != ColumnSize; ++index)
	{
		auto& ref = (*this)(row_number, index);
		row.push_back(ref);
	}

	return row;
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::column(std::size_t col_number) -> Vector
{
	Vector column;
	column.reserve(RowSize);

	for (std::size_t index = 0; index != RowSize; ++index)
	{
		auto& ref = (*this)(index, col_number);
		column.push_back(ref);
	}

	return column;
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::use_solve_method(std::shared_ptr<SolveMethod> method) -> Mat&
{
	solver_strategy_ = method;
	return *this;
}

template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
auto Matrix<RowSize, ColumnSize, TypeElem>::solve(std::vector<TypeElem>& free_column) -> std::vector<TypeElem>
{
	return solver_strategy_->solve(free_column, *this);
}

//--------------  End  ---------------//



//------------  Begin  ---------------//
//----Slice Strategy Implementation---//

template<std::size_t Dimension, typename TypeElem>
auto MainDiagonal<Dimension, TypeElem>::extract(Mat& matrix) -> Vector
{
	Vector slice;
	slice.reserve(Dimension);

	for (std::size_t index = 0; index != Dimension; ++index)
	{
		auto& ref = matrix(index, index);
		slice.push_back(ref);
	}

	return slice;
}


template<std::size_t Dimension, typename TypeElem>
auto LowerDiagonal<Dimension, TypeElem>::extract(Mat& matrix) -> Vector
{
	Vector slice;
	slice.reserve(Dimension);

	for (std::size_t index = 0; index != Dimension - 1; ++index)
	{
		auto& ref = matrix(index + 1, index);

		slice.push_back(ref);
	}

	return slice;
}


template<std::size_t Dimension, typename TypeElem>
auto UpperDiagonal<Dimension, TypeElem>::extract(Mat& matrix) -> Vector
{
	Vector slice;
	slice.reserve(Dimension);

	for (std::size_t index = 0; index != Dimension - 1; ++index)
	{
		auto& ref = matrix(index, index + 1);
		slice.push_back(ref);
	}

	return slice;
}

//--------------  End  ---------------//
