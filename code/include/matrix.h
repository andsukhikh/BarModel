#ifndef MATRIX_NEW_HPP
#define MATRIX_NEW_HPP

#include <vector>
#include <iostream>
#include <memory>


class Ordinary
{
	std::size_t row_size_;
	std::size_t column_size_;

	std::vector<double>			matrix_;
	std::vector<std::size_t>	index_of_first_row_elem_;
public:
	Ordinary(std::size_t row_size, std::size_t column_size);

	double& operator() (std::size_t row, std::size_t column);
};


class Tridiagonal
{
	friend class ShuttleMethod;
private:
	using UniquePtrSolveMethod = std::unique_ptr<class ISolveStrategyTridiagonalMatrix>;
private:
	std::size_t dimension_;

	double zero_elem_;

	std::vector<double>		main_diagonal_;
	std::vector<double>		upper_diagonal_;
	std::vector<double>		down_diagonal_;
	double other_elems_;

	UniquePtrSolveMethod solve_method_;

public:
	Tridiagonal(std::size_t size);

	double& operator() (std::size_t row, std::size_t column);
	const double& operator() (std::size_t row, std::size_t column) const;

	std::size_t dimension() const;
	
	Tridiagonal& set_solution_method(UniquePtrSolveMethod&& solve_method);

	std::vector<double> solve(std::vector<double> free_column);

	friend std::ostream& operator<< (std::ostream& output_stream, const Tridiagonal& this_matrix);

};


class ISolveStrategyTridiagonalMatrix
{
public:
	virtual ~ISolveStrategyTridiagonalMatrix() = default;
	virtual std::vector<double> solve(std::vector<double>& free_column, Tridiagonal& matrix) = 0;
};


class ShuttleMethod : public ISolveStrategyTridiagonalMatrix
{
public:
	std::vector<double> solve(std::vector<double>& free_column, Tridiagonal& matrix) override;
};


template <typename MatrixType>
class Matrix : public MatrixType
{};


#endif 

