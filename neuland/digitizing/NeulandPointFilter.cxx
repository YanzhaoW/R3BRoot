#include "NeulandPointFilter.h"
#include <iostream>
#include <utility>

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

void NeulandPointFilter::apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                                      R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                                      BitSetParticle FilteredParticles)
{
    for (const auto& neuland_point : neuland_points)
    {
        if (PidToBitSetParticle.find(neuland_point.GetPID()) == PidToBitSetParticle.end())
        {
            if (CheckCriteria(BitSetParticle::other, FilteredParticles))
            {
                filtered_neuland_points.get().emplace_back(neuland_point);
            }
        }
        else
        {
            if (CheckCriteria(PidToBitSetParticle.at(neuland_point.GetPID()), FilteredParticles))
            {
                filtered_neuland_points.get().emplace_back(neuland_point);
            }
        }
    }
}

auto NeulandPointFilter::Init() -> InitStatus
{
    neuland_points_.init();
    filtered_neuland_points_.init();

    return kSUCCESS;
}

void NeulandPointFilter::Exec(Option_t* /*option*/)
{
    filtered_neuland_points_.clear();
    apply_filter(neuland_points_, filtered_neuland_points_, ~BitSetParticle::none);
}
