#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <memory>

struct BoundaryCondition
{
	virtual ~BoundaryCondition() = default;
	virtual double at_point(const double temperature) = 0;
};


class FirstBoundaryCondition : public BoundaryCondition
{
	const double external_temperature_;
public:
	FirstBoundaryCondition(double external_temperature);

	double at_point(const double temperature) override;
};


class SecondBoundaryCondition : public BoundaryCondition
{
	const double flux_;
public:
	SecondBoundaryCondition(double flux);

	double at_point(const double temperature) override;
};


class ThirdBoundaryCondition : public BoundaryCondition
{
	const double heat_transfer_coefficient_;
	const double external_temperature_;
public:
	ThirdBoundaryCondition(double heat_transfer_coefficient, double external_temperature);

	double at_point(const double temperature) override;
};


struct Boundary
{
	std::shared_ptr<BoundaryCondition> left_boundary_;
	std::shared_ptr<BoundaryCondition> right_boundary_;
	std::shared_ptr<BoundaryCondition> up_boundary_;
	std::shared_ptr<BoundaryCondition> down_boundary_;
};


struct LowerRight
{
	static const bool check(std::size_t x_index, std::size_t y_index);
};

#endif // !BOUNDARY_H

