#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <memory>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <stdexcept>


template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
class SliceStrategy;


template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem>
class MatrixSolverStrategy;


template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem = double>
class Matrix
{
	using Ref					= std::reference_wrapper<TypeElem>;
	using Vector				= std::vector<Ref>;
	using Mat					= Matrix<RowSize, ColumnSize, TypeElem>;
	using SolveMethod			= MatrixSolverStrategy<RowSize, ColumnSize, TypeElem>;

	std::vector<TypeElem>					matrix_;
	std::vector<std::size_t>				row_first_elem_index_;

	std::shared_ptr<SolveMethod>			solver_strategy_;
public:
	Matrix();

	TypeElem& operator() (std::size_t row_number, std::size_t column_number);
	Vector slice(std::string_view diagonal_name);
	Vector slice(SliceStrategy<RowSize, ColumnSize, TypeElem>&& strategy);

	Vector row(std::size_t number_row);
	Vector column(std::size_t number_col);

	Mat& use_solve_method(std::shared_ptr<SolveMethod> method);
	std::vector<TypeElem> solve(std::vector<TypeElem>& free_column);
};


template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem = double>
class SliceStrategy
{
	using Ref			= std::reference_wrapper<TypeElem>;
	using Vector		= std::vector<Ref>;
	using Mat			= Matrix<RowSize, ColumnSize, TypeElem>;
public:
	virtual ~SliceStrategy() = default;
	virtual Vector extract(Mat& matrix) = 0;
};


template<std::size_t Dimension, typename TypeElem = double>
class MainDiagonal : public SliceStrategy<Dimension, Dimension, TypeElem>
{
	using Ref			= std::reference_wrapper<TypeElem>;
	using Vector		= std::vector<Ref>;
	using Mat			= Matrix<Dimension, Dimension, TypeElem>;
public:
	Vector extract(Mat& martix) override;
};


template<std::size_t Dimension, typename TypeElem = double>
class UpperDiagonal : public SliceStrategy<Dimension, Dimension, TypeElem>
{
	using Ref				= std::reference_wrapper<TypeElem>;
	using Vector			= std::vector<Ref>;
	using Mat				= Matrix<Dimension, Dimension, TypeElem>;
public:
	Vector extract(Mat& martix) override;
};


template<std::size_t Dimension, typename TypeElem = double>
class LowerDiagonal : public SliceStrategy<Dimension, Dimension, TypeElem>
{
	using Ref					= std::reference_wrapper<TypeElem>;
	using Vector				= std::vector<Ref>;
	using Mat					= Matrix<Dimension, Dimension, TypeElem>;
public:
	Vector extract(Mat& martix) override;
};


template<std::size_t RowSize, std::size_t ColumnSize, typename TypeElem = double>
class MatrixSolverStrategy
{
	using Mat = Matrix<RowSize, ColumnSize, TypeElem>;
public:
	virtual ~MatrixSolverStrategy() = default;
	virtual std::vector<TypeElem> solve(std::vector<TypeElem>& free_column, Mat& matrix) = 0;
};


template<std::size_t Dimension, typename TypeElem = double>
class TridiagonalMatrix : public MatrixSolverStrategy<Dimension, Dimension, TypeElem>
{
	using Mat		= Matrix<Dimension, Dimension, TypeElem>;
	using Ref		= std::reference_wrapper<TypeElem>;
	using Vector	= std::vector<Ref>;

	Vector lower_diagonal_;
	Vector main_diagonal_;
	Vector upper_diagonal_;
public:
	TridiagonalMatrix();

	std::vector<TypeElem> solve(std::vector<TypeElem>& free_column, Mat& matrix) override;
};

#include "matrix.cpp"

#endif // !MATRIX_HPP

