#pragma once
#include <TRandom.h>

#include <Math/GenVector/Cartesian3D.h>

auto energy_dist(TRandom* random)->double
{
    auto mean = double{ 10000. };
    auto sigma = double{ 10. };
    double energy = random->Gaus(mean, sigma);
    return energy;
}

auto point_dist(TRandom* random) -> ROOT::Math::Cartesian3D<double>
{
    auto xmin = double{ 0. };
    auto xmax = double{ 10. };
    auto ymin = double{ 0. };
    auto ymax = double{ 10. };
    auto zmin = double{ 0. };
    auto zmax = double{ 10. };
    auto box_points = ROOT::Math::Cartesian3D<double>{};
    box_points.SetX(random->Uniform(xmin, xmax));
    box_points.SetY(random->Uniform(ymin, ymax));
    box_points.SetZ(random->Uniform(zmin, zmax));
    return box_points;
}

double angle_dist(TRandom* random)
{
    double angle = asin(sqrt(random->Uniform()));
    return angle;
}
