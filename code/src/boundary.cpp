#include "boundary.h"

// FirstBoundaryCondition::FirstBoundaryCondition(double temperature)
//     : temperature_(temperature)
// {}

// double FirstBoundaryCondition::temperature()
// {
//     return temperature_;
// }

SecondBoundaryCondition::SecondBoundaryCondition(double flux)
    : flux_(flux)
{}

double SecondBoundaryCondition::at_point(const double temperature)
{
    return flux_;
}

double SecondBoundaryCondition::heat_transfer_coeff()
{
    return 0.0;
}

double SecondBoundaryCondition::external_temperature()
{
    return 0.0;
}


ThirdBoundaryCondition::ThirdBoundaryCondition(double heat_transfer_coefficient, double external_temperature)
    : heat_transfer_coefficient_(heat_transfer_coefficient)
    , external_temperature_(external_temperature)
{}

double ThirdBoundaryCondition::at_point(const double temperature)
{
    return heat_transfer_coefficient_ * (temperature - external_temperature_);
}


double ThirdBoundaryCondition::heat_transfer_coeff()
{
    return heat_transfer_coefficient_;
}

double ThirdBoundaryCondition::external_temperature()
{
    return external_temperature_;
}

