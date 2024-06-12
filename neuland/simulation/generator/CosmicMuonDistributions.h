#pragma once
#include <Math/GenVector/Cartesian3D.h>
#include <R3BNeulandCommon.h>
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
            /*   auto angle = asin(sqrt((M_PI / 2) * rd_engine_->Uniform())); */
            auto angle = rd_engine_->Gaus(0., 1.);
            return angle;
        };
    };

    struct DetectorBoxSize
    {
      public:
        double xmin{};
        double xmax{};
        double ymin{};
        double ymax{};
        double zmin{};
        double zmax{};
    };

    class PositionDist
    {
      public:
        void set_box_size(DetectorBoxSize detector_box_size)
        {
            detector_box_size_.xmin = detector_box_size.xmin;
            detector_box_size_.xmax = detector_box_size.xmax;
            detector_box_size_.ymin = detector_box_size.ymin;
            detector_box_size_.ymax = detector_box_size.ymax;
            detector_box_size_.zmin = detector_box_size.zmin;
            detector_box_size_.zmax = detector_box_size.zmax;
        }

        auto operator()(TRandom* rd_engine_) const -> ROOT::Math::Cartesian3D<double>
        {
            auto box_positions = ROOT::Math::Cartesian3D<double>{};
            box_positions.SetX(rd_engine_->Uniform(detector_box_size_.xmin, detector_box_size_.xmax));
            box_positions.SetY(rd_engine_->Uniform(detector_box_size_.ymin, detector_box_size_.ymax));
            box_positions.SetZ(rd_engine_->Uniform(detector_box_size_.zmin, detector_box_size_.zmax));
            return box_positions;
        }

      private:
        DetectorBoxSize detector_box_size_{};
    };
} // namespace R3B::Neuland
