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
	std::size_t dimension_;

	double zero_elem_;

	std::vector<double>		main_diagonal_;
	std::vector<double>		upper_diagonal_;
	std::vector<double>		down_diagonal_;
	double other_elems_;

	std::unique_ptr<class ISolveStrategyTridiagonalMatrix> solve_method_;

public:
	Tridiagonal(std::size_t size);

	double& operator() (std::size_t row, std::size_t column);
	const double& operator() (std::size_t row, std::size_t column) const;

	std::size_t dimension() const;
	
	Tridiagonal& set_solution_method(ISolveStrategyTridiagonalMatrix&& solve_method);

	std::vector<double> solve(std::vector<double> free_column);

	friend std::ostream& operator<< (std::ostream& output_stream, const Tridiagonal& this_matrix);
private:
	const bool is_diagonal(std::size_t row, std::size_t column) const;
	const bool is_upper_diagonal(std::size_t row, std::size_t column) const;
	const bool is_lower_diagonal(std::size_t row, std::size_t column) const;
};


class ISolveStrategyTridiagonalMatrix
{
public:
	virtual ~ISolveStrategyTridiagonalMatrix() = default;
	virtual std::vector<double> solve(std::vector<double>& free_column, Tridiagonal& matrix) = 0;
	virtual std::unique_ptr<ISolveStrategyTridiagonalMatrix> return_copy_ptr() = 0;
};


class ShuttleMethod : public ISolveStrategyTridiagonalMatrix
{
public:
	std::vector<double> solve(std::vector<double>& free_column, Tridiagonal& matrix) override;
	std::unique_ptr<ISolveStrategyTridiagonalMatrix> return_copy_ptr() override;
};

#endif 

