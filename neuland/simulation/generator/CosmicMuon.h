/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2024 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#pragma once
#include "FairPrimaryGenerator.h"
#include "RtypesCore.h"
#include "TRandom.h"
#include <FairGenerator.h>
#include <Math/GenVector/Cartesian3D.h>
#include <Math/GenVector/Polar3D.h>
#include <Math/GenVector/PxPyPzE4D.h>
#include <R3BNeulandCommon.h>
#include <TRandom3.h>
#include <cmath>
#include <fmt/format.h>
#include <memory>
#include <utility>

namespace R3B::Neuland
{
    struct AngleInfo
    {
        double sin_phi;
        double sin_theta;
        double cos_phi;
        double cos_theta;
    };

    template <typename AngleDist, typename EnergyDist, typename PointDist>
    class TrackGenerator : FairGenerator
    {
      public:
        TrackGenerator(AngleDist angle_dist, EnergyDist energy_dist, PointDist point_dist)
            : angle_dist_{ angle_dist }
            , energy_dist_{ energy_dist }
            , point_dist_{ point_dist }
        {
        }

        void set_detector_size(double detector_size) { detector_size_ = detector_size; }
        void set_rd_engine(TRandom* user_rd_engine) { rd_engine_ = user_rd_engine; }

      private:
        static constexpr auto CLight = ::Neuland::CLight;
        double detector_size_{ 50.0 };

        AngleDist angle_dist_{};
        EnergyDist energy_dist_{};
        PointDist point_dist_{};
        TRandom* rd_engine_{ gRandom };

        auto rd_num_gen_point() -> ROOT::Math::Cartesian3D<double>;
        auto rd_num_gen_angles(AngleDist angle_dist_) -> ROOT::Math::Polar3D<double>;
        auto rd_num_gen_energy() -> double;
        auto calculate_abs_momentum(double kinetic_energy) -> double { return kinetic_energy / CLight; };
        auto calculate_momentum_energy(double kinetic_energy, AngleInfo& angle_info) -> ROOT::Math::PxPyPzE4D<double>;

        auto calculate_external_position_momentum(PointDist point_dist_, AngleDist angle_dist_, EnergyDist energy_dist_)
            -> std::pair<ROOT::Math::PxPyPzE4D<double>, ROOT::Math::Cartesian3D<double>>;

        auto ReadEvent(FairPrimaryGenerator* prim_gen) -> Bool_t override
        {

            auto position_momentum = std::pair<ROOT::Math::PxPyPzE4D<double>, ROOT::Math::Cartesian3D<double>>{
                calculate_external_position_momentum(point_dist_, angle_dist_, energy_dist_)
            };
            prim_gen->AddTrack(13,
                               position_momentum.first.Px(),
                               position_momentum.first.Py(),
                               position_momentum.first.Pz(),
                               position_momentum.second.X(),
                               position_momentum.second.Y(),
                               position_momentum.second.Z());

            return true;
        };
    };

    template <typename AngleDist, typename EnergyDist, typename PointDist>
    auto TrackGenerator<AngleDist, EnergyDist, PointDist>::rd_num_gen_angles(AngleDist angle_dist)
        -> ROOT::Math::Polar3D<double>
    {
        auto angles = ROOT::Math::Polar3D<double>{};
        angles.SetPhi(rd_engine_->Uniform(0., M_PI));
        angles.SetTheta(angle_dist(rd_engine_));
        return angles;
    }

    template <typename AngleDist, typename EnergyDist, typename PointDist>
    auto TrackGenerator<AngleDist, EnergyDist, PointDist>::calculate_momentum_energy(double kinetic_energy,
                                                                                     AngleInfo& angle_info)
        -> ROOT::Math::PxPyPzE4D<double>
    {
        auto momentum_energy = ROOT::Math::PxPyPzE4D<double>{ 0, 0, 0, kinetic_energy };
        auto abs_momentum = double{ calculate_abs_momentum(kinetic_energy) };
        momentum_energy.SetPx(abs_momentum * angle_info.sin_theta * angle_info.cos_phi);
        momentum_energy.SetPy(abs_momentum * angle_info.sin_theta * angle_info.sin_phi);
        momentum_energy.SetPz(abs_momentum * angle_info.cos_theta);
        return momentum_energy;
    }

    template <typename AngleDist, typename EnergyDist, typename PointDist>
    auto TrackGenerator<AngleDist, EnergyDist, PointDist>::calculate_external_position_momentum(PointDist point_dist,
                                                                                                AngleDist angle_dist,
                                                                                                EnergyDist energy_dist)
        -> std::pair<ROOT::Math::PxPyPzE4D<double>, ROOT::Math::Cartesian3D<double>>
    {
        auto const point = point_dist(rd_engine_);
        auto const angles = rd_num_gen_angles(angle_dist);
        auto const energy = energy_dist(rd_engine_);

        auto angle_info = AngleInfo{};
        angle_info.sin_phi = std::sin(angles.phi());
        angle_info.cos_phi = std::cos(angles.phi());
        angle_info.sin_theta = std::sin(angles.theta());
        angle_info.cos_theta = std::cos(angles.theta());

        std::pair<ROOT::Math::PxPyPzE4D<double>, ROOT::Math::Cartesian3D<double>> position_momentum{};

        position_momentum.second.SetX(point.x() - angle_info.sin_theta * angle_info.cos_phi * detector_size_);
        position_momentum.second.SetY(point.y() - angle_info.sin_theta * angle_info.sin_phi * detector_size_);
        position_momentum.second.SetZ(point.z() - angle_info.cos_theta * detector_size_);
        position_momentum.first = calculate_momentum_energy(energy, angle_info);

        return position_momentum;
    }
} // namespace R3B::Neuland
