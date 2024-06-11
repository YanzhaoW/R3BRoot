#pragma once
#include <Math/GenVector/Cartesian3D.h>
#include <TRandom.h>

namespace R3B::Neuland
{
    class EnergyDist
    {

      public:
        auto operator()(TRandom* rd_engine_) const -> double
        {
            auto mean = double{ 10000. };
            auto sigma = double{ 10. };
            double energy = rd_engine_->Gaus(mean, sigma);
            return energy;
        };
    };

    class AngleDist
    {

      public:
        auto operator()(TRandom* rd_engine_) const -> double
        {
            double angle = asin(sqrt(rd_engine_->Uniform()));
            return angle;
        };
    };

    class PointDist
    {

      public:
        auto operator()(TRandom* rd_engine_) const -> ROOT::Math::Cartesian3D<double>
        {
            auto xmin = double{ 0. };
            auto xmax = double{ 10. };
            auto ymin = double{ 0. };
            auto ymax = double{ 10. };
            auto zmin = double{ 0. };
            auto zmax = double{ 10. };
            auto box_points = ROOT::Math::Cartesian3D<double>{};
            box_points.SetX(rd_engine_->Uniform(xmin, xmax));
            box_points.SetY(rd_engine_->Uniform(ymin, ymax));
            box_points.SetZ(rd_engine_->Uniform(zmin, zmax));
            return box_points;
        }
    };
} // namespace R3B::Neuland
