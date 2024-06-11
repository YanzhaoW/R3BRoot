#pragma once
#include <Math/GenVector/Cartesian3D.h>
#include <TRandom.h>

namespace R3B::Neuland
{
    class EnergyDist
    {
      public:
        void set_mean_sigma(double mean, double sigma)
        {
            mean_ = mean;
            sigma_ = sigma;
        }

        auto operator()(TRandom* rd_engine_) const -> double
        {
            auto energy = rd_engine_->Gaus(mean_, sigma_);
            return energy;
        };

      private:
        double mean_{ 10000. };
        double sigma_{ 10. };
    };

    class AngleDist
    {
      public:
        auto operator()(TRandom* rd_engine_) const -> double
        {
            auto angle = asin(sqrt(rd_engine_->Uniform()));
            return angle;
        };
    };

    class PositionDist
    {
      public:
        void set_box_size(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
        {
            xmin_ = xmin, xmax_ = xmax, ymin_ = ymin, ymax_ = ymax, zmin_ = zmin, zmax_ = zmax;
        }

        auto operator()(TRandom* rd_engine_) const -> ROOT::Math::Cartesian3D<double>
        {
            auto box_positions = ROOT::Math::Cartesian3D<double>{};
            box_positions.SetX(rd_engine_->Uniform(xmin_, xmax_));
            box_positions.SetY(rd_engine_->Uniform(ymin_, ymax_));
            box_positions.SetZ(rd_engine_->Uniform(zmin_, zmax_));
            return box_positions;
        }

      private:
        double xmin_ = { 0. };
        double xmax_ = { 10. };
        double ymin_ = { 0. };
        double ymax_ = { 10. };
        double zmin_ = { 0. };
        double zmax_ = { 10. };
    };
} // namespace R3B::Neuland
