#ifndef MATRIX_NEW_HPP
#define MATRIX_NEW_HPP

#include <vector>
#include <iostream>


template <typename DataType = double>
class Ordinary
{
	std::size_t row_size_;
	std::size_t column_size_;

	std::vector<DataType>		matrix_;
	std::vector<std::size_t>	index_of_first_row_elem_;
public:
	Ordinary(std::size_t row_size, std::size_t column_size);

	DataType& operator() (std::size_t row, std::size_t column);
};

template<typename> class Tridiagonal;

template <typename DataType = double>
class ISolveStrategyTridiagonalMatrix
{
public:
	virtual ~ISolveStrategyTridiagonalMatrix() = default;
	virtual std::vector<DataType> solve(std::vector<DataType>& free_column, Tridiagonal<DataType>& matrix) = 0;
};


template <typename DataType = double>
class ShuttleMethod : public ISolveStrategyTridiagonalMatrix<DataType>
{
public:
	std::vector<DataType> solve(std::vector<DataType>& free_column, Tridiagonal<DataType>& matrix) override;
};


template <typename DataType = double>
class Tridiagonal
{
private:
	friend class ShuttleMethod<DataType>;

	using ThisMatrixType = Tridiagonal<DataType>;
	using UniquePtrSolveMethod = std::unique_ptr<ISolveStrategyTridiagonalMatrix<DataType>>;
private:
	std::size_t dimension_;

	DataType zero_elem_;

	std::vector<DataType>		main_diagonal_;
	std::vector<DataType>		upper_diagonal_;
	std::vector<DataType>		down_diagonal_;
	DataType other_elems_;

	UniquePtrSolveMethod solve_method_;

public:
	Tridiagonal(std::size_t size);

	DataType& operator() (std::size_t row, std::size_t column);
	const DataType& operator() (std::size_t row, std::size_t column) const;
	
	Tridiagonal<DataType>& set_solve_method(UniquePtrSolveMethod&& solve_method);

	std::vector<DataType> solve(std::vector<DataType> free_column);

	template <typename Type = double>
	friend std::ostream& operator<< (std::ostream& output_stream, const Tridiagonal<Type>& this_matrix);

	std::size_t dimension() const;
};


//template <typename MatrixType = Ordinary, typename DataType = double>
//class Matrix_New : public MatrixType
//{
//	using Vector = std::vector<DataType>
//public:
//	DataType& operator() (std::size_t row, std::size_t column)
//	{
//		//if (row < RowSize && column < ColumnSize)
//		//{
//		//	auto row_offset = row_first_elem_index_.at(row);
//		//	return matrix_.at(row_offset + column);
//		//}
//		//throw std::out_of_range("Row or column index out of bounds");
//	}
//
//	
//};

#include "matrix_new.cpp"

#endif 

