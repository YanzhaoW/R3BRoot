#include "NeulandPointFilter.h"
#include <iostream>
#include <utility>

using ParticleUType = std::underlying_type<BitsetParticle>::type;

constexpr auto ParticleToBitset(BitsetParticle particle)
{
    return std::bitset<ParticleBitsetSize>{ static_cast<ParticleUType>(particle) };
}

auto BitsetToParticle(std::bitset<ParticleBitsetSize> bits) -> BitsetParticle
{
    return static_cast<BitsetParticle>(static_cast<uint32_t>(bits.to_ulong()));
}

auto CheckCriteria(BitsetParticle particle, BitsetParticle criteria) -> bool
{
    return (ParticleToBitset(particle) & ParticleToBitset(criteria)) == ParticleToBitset(particle);
}

auto operator|(BitsetParticle left, BitsetParticle right) -> BitsetParticle
{
    auto left_bitset = ParticleToBitset(left);
    auto right_bitset = ParticleToBitset(right);
    return BitsetToParticle(left_bitset | right_bitset);
}

auto operator&(BitsetParticle left, BitsetParticle right) -> BitsetParticle
{
    auto left_bitset = ParticleToBitset(left);
    auto right_bitset = ParticleToBitset(right);
    return BitsetToParticle(left_bitset & right_bitset);
}

auto operator~(BitsetParticle particle) -> BitsetParticle { return BitsetToParticle(~ParticleToBitset(particle)); }

constexpr auto proton_pid = 2212;
constexpr auto neutron_pid = 2112;
constexpr auto electron_pid = 11;
constexpr auto positron_pid = -11;
constexpr auto alpha_pid = 1000040020;
constexpr auto gamma_pid = 22;

auto NeulandPointFilter::check_pid_for_criteria(int pid, BitsetParticle criteria) -> bool
{
    return ((CheckCriteria(BitsetParticle::proton, criteria) && pid == proton_pid) ||
            (CheckCriteria(BitsetParticle::neutron, criteria) && pid == neutron_pid) ||
            (CheckCriteria(BitsetParticle::electron, criteria) && pid == electron_pid) ||
            (CheckCriteria(BitsetParticle::positron, criteria) && pid == positron_pid) ||
            (CheckCriteria(BitsetParticle::alpha, criteria) && pid == alpha_pid) ||
            (CheckCriteria(BitsetParticle::gamma, criteria) && pid == gamma_pid));
}

void NeulandPointFilter::SetFilter(FilteringMode filtering_mode) { filtering_mode_ = filtering_mode; }

void NeulandPointFilter::apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                                      R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                                      BitsetParticle AllowedParticles)
{
    for (const auto& neuland_point : neuland_points)
    {
        if (check_pid_for_criteria(neuland_point.GetPID(), AllowedParticles))
        {
            filtered_neuland_points.get().emplace_back(neuland_point);
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
    switch (filtering_mode_)
    {
        case FilteringMode::none:
            for (const auto& neuland_point : neuland_points_)
            {
                filtered_neuland_points_.get().emplace_back(neuland_point);
            }
            break;
        case FilteringMode::all_but_protons:
            apply_filter(neuland_points_, filtered_neuland_points_, BitsetParticle::proton);
            break;
        case FilteringMode::custom:
            apply_filter(neuland_points_, filtered_neuland_points_, ~BitsetParticle::proton & ~BitsetParticle::electron);
            break;
        default:
            std::cout << "Unexpected FilteringMode\n";
    }
}
