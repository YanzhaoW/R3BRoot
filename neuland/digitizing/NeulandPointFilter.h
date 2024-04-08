#pragma once
#include "FairTask.h"
#include "R3BIOConnector.h"
#include "R3BNeulandPoint.h"
#include <bitset>

namespace R3B::Neuland
{
    enum class BitSetParticle : uint32_t
    {
        none = 0x0000,
        proton = 0x0001,
        neutron = 0x0002,
        electron = 0x0004,
        positron = 0x0008,
        alpha = 0x0010,
        gamma = 0x0020,
        other = 0x40000000
    };

    const std::unordered_map<int, BitSetParticle> PidToBitSetParticleHash = {
        { 2212, BitSetParticle::proton },  { 2112, BitSetParticle::neutron },     { 11, BitSetParticle::electron },
        { -11, BitSetParticle::positron }, { 1000040020, BitSetParticle::alpha }, { 22, BitSetParticle::gamma },
        { 0, BitSetParticle::none }
    };

    constexpr auto ParticleBitsetSize = 32U;

    constexpr auto ParticleToBitSet(BitSetParticle particle);
    auto BitSetToParticle(std::bitset<ParticleBitsetSize> bits) -> BitSetParticle;
    auto CheckCriteria(BitSetParticle particle, BitSetParticle criteria) -> bool;
    auto PidToBitSetParticle(int pid) -> BitSetParticle;

    auto operator|(BitSetParticle left, BitSetParticle right) -> BitSetParticle;
    auto operator&(BitSetParticle left, BitSetParticle right) -> BitSetParticle;
    auto operator~(BitSetParticle particle) -> BitSetParticle;

} // namespace R3B::Neuland
class NeulandPointFilter
{
  public:
    NeulandPointFilter() = default;
    void SetFilter(R3B::Neuland::BitSetParticle filtered_particles);
    [[nodiscard]] auto GetFilter() const -> R3B::Neuland::BitSetParticle { return filtered_particles_; }
    auto ShouldNeulandPointBeFiltered(const R3BNeulandPoint& neuland_point) -> bool;

  private:
    R3B::Neuland::BitSetParticle filtered_particles_ = R3B::Neuland::BitSetParticle::none;
};
