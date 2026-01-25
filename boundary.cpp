#include "boundary.h"

FirstBoundaryCondition::FirstBoundaryCondition(double external_temperature)
    : external_temperature_(external_temperature)
{}

double FirstBoundaryCondition::at_point(const double temperature)
{
    return external_temperature_;
}

SecondBoundaryCondition::SecondBoundaryCondition(double flux)
    : flux_(flux)
{}

double SecondBoundaryCondition::at_point(const double temperature)
{
    return flux_;
}

ThirdBoundaryCondition::ThirdBoundaryCondition(double heat_transfer_coefficient, double external_temperature)
    : heat_transfer_coefficient_(heat_transfer_coefficient)
    , external_temperature_(external_temperature)
{}

double ThirdBoundaryCondition::at_point(const double temperature)
{
    return heat_transfer_coefficient_ * (temperature - external_temperature_);
}

