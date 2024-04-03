#pragma once
#include "FairTask.h"
#include "R3BIOConnector.h"
#include "R3BNeulandPoint.h"
#include <bitset>

enum class BitSetParticle : uint32_t
{
    none = 0x0000,
    proton = 0x0001,
    neutron = 0x0002,
    electron = 0x0004,
    positron = 0x0008,
    alpha = 0x0010,
    gamma = 0x0020,
    other = 0x0040
};

const std::unordered_map<int,BitSetParticle> PidToBitSetParticle = {
    {2212,BitSetParticle::proton},
    {2112, BitSetParticle::neutron},
    {11, BitSetParticle::electron},
    {-11, BitSetParticle::positron},
    {1000040020, BitSetParticle::alpha},
    {22, BitSetParticle::gamma},
    {0, BitSetParticle::none}
};

constexpr auto ParticleBitsetSize = 32U;

constexpr auto ParticleToBitSet(BitSetParticle particle);
auto BitSetToParticle(std::bitset<ParticleBitsetSize> bits) -> BitSetParticle;
auto CheckCriteria(BitSetParticle particle, BitSetParticle criteria) -> bool;

auto operator|(BitSetParticle left, BitSetParticle right) -> BitSetParticle;
auto operator&(BitSetParticle left, BitSetParticle right) -> BitSetParticle;
auto operator~(BitSetParticle particle) -> BitSetParticle;


class NeulandPointFilter : public FairTask
{
  public:
    NeulandPointFilter() = default;

  private:
    auto Init() -> InitStatus override;
    void Exec(Option_t* /*option*/) override;

    static auto check_pid_for_criteria(int pid, BitSetParticle criteria) -> bool;

    static void apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                             R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                             BitSetParticle FilteredParticles);


    R3B::InputVectorConnector<R3BNeulandPoint> neuland_points_{ "NeulandPoints" };
    R3B::OutputVectorConnector<R3BNeulandPoint> filtered_neuland_points_{ "FilteredNeulandPoints" };
};
