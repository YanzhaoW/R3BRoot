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
        double sin_phi{};
        double sin_theta{};
        double cos_phi{};
        double cos_theta{};
    };

    template <typename AngleDist, typename EnergyDist, typename PositionDist>
    class TrackGenerator : public FairGenerator
    {
      public:
        TrackGenerator(const AngleDist& angle_dist, const EnergyDist& energy_dist, const PositionDist& position_dist)
            : angle_dist_{ angle_dist }
            , energy_dist_{ energy_dist }
            , position_dist_{ position_dist }
        {
        }

        void set_detector_size(double detector_size) { detector_size_ = detector_size; }
        void set_rd_engine(TRandom* user_rd_engine) { rd_engine_ = user_rd_engine; }
        void set_PID(int PID) { PID_ = PID; };

      private:
        using MomentumPosition = std::pair<ROOT::Math::PxPyPzE4D<double>, ROOT::Math::Cartesian3D<double>>;
        using Momentum = ROOT::Math::PxPyPzE4D<double>;
        using AngleRadius = ROOT::Math::Polar3D<double>;
        static constexpr auto CLight = ::Neuland::CLight;
        double detector_size_{ 50.0 };
        int PID_{ 13 };

        AngleDist angle_dist_{};
        EnergyDist energy_dist_{};
        PositionDist position_dist_{};
        TRandom* rd_engine_{ gRandom };

        auto rd_num_gen_angles(const AngleDist& angle_dist) -> AngleRadius;
        auto calculate_abs_momentum(const double& kinetic_energy) -> double { return kinetic_energy / CLight; };
        auto calculate_momentum_energy(const double& kinetic_energy, const AngleInfo& angle_info) -> Momentum;

        auto calculate_external_position_momentum(const AngleDist& angle_dist,
                                                  const EnergyDist& energy_dist,
                                                  const PositionDist& position_dist) -> MomentumPosition;

        auto ReadEvent(FairPrimaryGenerator* prim_gen) -> Bool_t override
        {

            auto position_momentum =
                MomentumPosition{ calculate_external_position_momentum(angle_dist_, energy_dist_, position_dist_) };
            prim_gen->AddTrack(PID_,
                               position_momentum.first.Px(),
                               position_momentum.first.Py(),
                               position_momentum.first.Pz(),
                               position_momentum.second.X(),
                               position_momentum.second.Y(),
                               position_momentum.second.Z());

            return true;
        };
    };

    template <typename AngleDist, typename EnergyDist, typename PositionDist>
    auto TrackGenerator<AngleDist, EnergyDist, PositionDist>::rd_num_gen_angles(const AngleDist& angle_dist)
        -> AngleRadius
    {
        auto angles = AngleRadius{};
        angles.SetPhi(rd_engine_->Uniform(0., M_PI));
        angles.SetTheta(angle_dist(rd_engine_));
        return angles;
    }

    template <typename AngleDist, typename EnergyDist, typename PositionDist>
    auto TrackGenerator<AngleDist, EnergyDist, PositionDist>::calculate_momentum_energy(const double& kinetic_energy,
                                                                                        const AngleInfo& angle_info)
        -> Momentum
    {
        auto momentum_energy = Momentum{ 0, 0, 0, kinetic_energy };
        auto abs_momentum = double{ calculate_abs_momentum(kinetic_energy) };
        momentum_energy.SetPx(abs_momentum * angle_info.sin_theta * angle_info.cos_phi);
        momentum_energy.SetPy(abs_momentum * angle_info.sin_theta * angle_info.sin_phi);
        momentum_energy.SetPz(abs_momentum * angle_info.cos_theta);
        return momentum_energy;
    }

    template <typename AngleDist, typename EnergyDist, typename PositionDist>
    auto TrackGenerator<AngleDist, EnergyDist, PositionDist>::calculate_external_position_momentum(
        const AngleDist& angle_dist,
        const EnergyDist& energy_dist,
        const PositionDist& position_dist) -> MomentumPosition
    {
        // auto const Position = position_dist(rd_engine_);
        // auto const angles = rd_num_gen_angles(angle_dist);
        // auto const energy = energy_dist(rd_engine_);
        auto const position = position_dist(rd_engine_);
        auto const angles = rd_num_gen_angles(angle_dist);
        auto const energy = energy_dist(rd_engine_);

        auto angle_info = AngleInfo{};
        angle_info.sin_phi = std::sin(angles.Phi());
        angle_info.cos_phi = std::cos(angles.Phi());
        angle_info.sin_theta = std::sin(angles.Theta());
        angle_info.cos_theta = std::cos(angles.Theta());

        auto position_momentum = MomentumPosition{};

        position_momentum.second.SetX(position.X() - angle_info.sin_theta * angle_info.cos_phi * detector_size_);
        position_momentum.second.SetY(position.Y() - angle_info.sin_theta * angle_info.sin_phi * detector_size_);
        position_momentum.second.SetZ(position.Z() - angle_info.cos_theta * detector_size_);
        position_momentum.first = calculate_momentum_energy(energy, angle_info);

        return position_momentum;
    }

    template <typename AngleDist, typename EnergyDist, typename PositionDist>
    auto CreateTrackGenerator(const AngleDist& angle_dist,
                              const EnergyDist& energy_dist,
                              const PositionDist& position_dist)
    {
        return std::make_unique<TrackGenerator<AngleDist, EnergyDist, PositionDist>>(
            angle_dist, energy_dist, position_dist);
    }

} // namespace R3B::Neuland
