#include "NeulandPointFilter.h"
#include <iostream>
#include <utility>
namespace R3B::Neuland
{
    using ParticleUType = std::underlying_type<BitSetParticle>::type;

    constexpr auto ParticleToBitSet(BitSetParticle particle)
    {
        return std::bitset<ParticleBitsetSize>{ static_cast<ParticleUType>(particle) };
    }

    auto BitSetToParticle(std::bitset<ParticleBitsetSize> bits) -> BitSetParticle
    {
        return static_cast<BitSetParticle>(static_cast<uint32_t>(bits.to_ulong()));
    }

    auto CheckCriteria(BitSetParticle particle, BitSetParticle criteria) -> bool
    {
        return (ParticleToBitSet(particle) & ParticleToBitSet(criteria)) == ParticleToBitSet(particle);
    }

    auto operator|(BitSetParticle left, BitSetParticle right) -> BitSetParticle
    {
        auto left_bitset = ParticleToBitSet(left);
        auto right_bitset = ParticleToBitSet(right);
        return BitSetToParticle(left_bitset | right_bitset);
    }

    auto operator&(BitSetParticle left, BitSetParticle right) -> BitSetParticle
    {
        auto left_bitset = ParticleToBitSet(left);
        auto right_bitset = ParticleToBitSet(right);
        return BitSetToParticle(left_bitset & right_bitset);
    }

    auto operator~(BitSetParticle particle) -> BitSetParticle { return BitSetToParticle(~ParticleToBitSet(particle)); }

    auto PidToBitSetParticle(int pid) -> BitSetParticle
    {
        // mesons have three digit pdgs
        if (pid > 99 and pid < 1000) //NOLINT
        {
            return BitSetParticle::meson;
        }
        auto pid_to_bitset_hash_iterator = PidToBitSetParticleHash.find(pid);

        if (pid_to_bitset_hash_iterator == PidToBitSetParticleHash.end())
        {
            return BitSetParticle::other;
        }

        return pid_to_bitset_hash_iterator->second;
    }
} // namespace R3B::Neuland

void NeulandPointFilter::SetFilter(R3B::Neuland::BitSetParticle filtered_particles)
{
    filtered_particles_ = filtered_particles;
}
void NeulandPointFilter::SetFilter(R3B::Neuland::BitSetParticle filtered_particles, double minimum_allowed_energy)
{
    filtered_particles_ = filtered_particles;
    minimum_allowed_energy_ = minimum_allowed_energy;
}

auto NeulandPointFilter::ShouldNeulandPointBeFiltered(const R3BNeulandPoint& neuland_point) -> bool
{
    return (
        R3B::Neuland::CheckCriteria(R3B::Neuland::PidToBitSetParticle(neuland_point.GetPID()), filtered_particles_) or
        (neuland_point.GetEnergyLoss() < minimum_allowed_energy_));
}
