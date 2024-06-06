#pragma once
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
#include "FairPrimaryGenerator.h"
template <typename AngleDist, typename EnergyDist, typename PointDist>
class TrackGenerator : FairGenerator
{
  public:
    TrackGenerator(AngleDist angle_dist, EnergyDist energy_dist, PointDist point_dist)
        : angle_dist_(angle_dist)
        , energy_dist_(energy_dist)
        , point_dist_(point_dist)
    {
    }

    void set_detector_size(double detector_size) { detector_size_ = detector_size; }
    void set_rd_engine(TRandom* user_rd_engine) { rd_engine_ = user_rd_engine; }

  private:
    double detector_size_{ 50.0 };

    ROOT::Math::Cartesian3D<double> position_{};
    ROOT::Math::PxPyPzE4D<double> momentum_energy_{};

    AngleDist angle_dist_;
    EnergyDist energy_dist_;
    PointDist point_dist_;
    /* TRandom3 rd_engine_; */
    TRandom* rd_engine_{ gRandom };

    auto rd_num_gen_point() -> ROOT::Math::Cartesian3D<double>;
    auto rd_num_gen_angles() -> ROOT::Math::Polar3D<double>;
    auto rd_num_gen_energy() -> double;
    auto calculate_abs_momentum(double kinetic_energy) -> double;
    auto calculate_momentum_energy(double kinetic_energy,
                                   double sin_theta,
                                   double sin_phi,
                                   double cos_theta,
                                   double cos_phi) -> void;

    void calculate_external_position();

    Bool_t ReadEvent(FairPrimaryGenerator* prim_gen) override
    {
        calculate_external_position();
        prim_gen->AddTrack(13,momentum_energy_.Px(),momentum_energy_.Py(),momentum_energy_.Pz(), position_.X(),  position_.Y(), position_.Z());

        return true;
    };
};

template <typename AngleDist, typename EnergyDist, typename PointDist>
auto TrackGenerator<AngleDist, EnergyDist, PointDist>::rd_num_gen_angles() -> ROOT::Math::Polar3D<double>
{
    ROOT::Math::Polar3D<double> angles{};
    angles.SetPhi(rd_engine_->Uniform(0., M_PI));
    angles.SetTheta(angle_dist_(rd_engine_));
    return angles;
}
template <typename AngleDist, typename EnergyDist, typename PointDist>
auto TrackGenerator<AngleDist, EnergyDist, PointDist>::calculate_abs_momentum(double kinetic_energy) -> double
{
    return kinetic_energy / Neuland::CLight;
}
template <typename AngleDist, typename EnergyDist, typename PointDist>
auto TrackGenerator<AngleDist, EnergyDist, PointDist>::calculate_momentum_energy(double kinetic_energy,
                                                                                 double sin_theta,
                                                                                 double sin_phi,
                                                                                 double cos_theta,
                                                                                 double cos_phi) -> void
{
    ROOT::Math::PxPyPzE4D<double> momentum_energy{ 0, 0, 0, kinetic_energy };
    double abs_momentum = calculate_abs_momentum(kinetic_energy);
    momentum_energy_.SetPx(abs_momentum * sin_theta * cos_phi);
    momentum_energy_.SetPy(abs_momentum * sin_theta * sin_phi);
    momentum_energy_.SetPz(abs_momentum * cos_theta);
}

template <typename AngleDist, typename EnergyDist, typename PointDist>
void TrackGenerator<AngleDist, EnergyDist, PointDist>::calculate_external_position()
{
    auto point = point_dist_(rd_engine_);
    auto angles = rd_num_gen_angles();
    auto energy = energy_dist_(rd_engine_);
    auto sin_phi = std::sin(angles.phi());
    auto cos_phi = std::cos(angles.phi());
    auto sin_theta = std::sin(angles.theta());
    auto cos_theta = std::cos(angles.theta());
    calculate_momentum_energy(energy, sin_theta, sin_phi, cos_theta, cos_phi);

    position_.SetX(point.x() - sin_theta * cos_phi * detector_size_);
    position_.SetY(point.y() - sin_theta * sin_phi * detector_size_);
    position_.SetZ(point.z() - cos_theta * detector_size_);
}
