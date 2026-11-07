#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <memory>

struct BoundaryCondition
{
	virtual ~BoundaryCondition() = default;
	virtual double at_point(const double temperature) = 0;

	virtual double heat_transfer_coeff() = 0;
	virtual double external_temperature() = 0;

};


// class FirstBoundaryCondition : public BoundaryCondition
// {
// 	const double temperature_;
// public:
// 	FirstBoundaryCondition(double temperature);

// 	double temperature();
// };

class SecondBoundaryCondition : public BoundaryCondition
{
	const double flux_;
public:
	SecondBoundaryCondition(double flux);

	double at_point(const double temperature) override;

	double heat_transfer_coeff() override;
	double external_temperature() override;
};


class ThirdBoundaryCondition : public BoundaryCondition
{
	const double heat_transfer_coefficient_;
	const double external_temperature_;
public:
	ThirdBoundaryCondition(double heat_transfer_coefficient, double external_temperature);

	double at_point(const double temperature) override;

	double heat_transfer_coeff() override;
	double external_temperature() override;
};


struct Boundary
{
	std::shared_ptr<BoundaryCondition> left_boundary		= {};
	std::shared_ptr<BoundaryCondition> right_boundary		= {};
	std::shared_ptr<BoundaryCondition> up_boundary			= {};
	std::shared_ptr<BoundaryCondition> down_boundary		= {};
};

#endif

