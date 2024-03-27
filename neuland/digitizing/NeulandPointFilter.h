#pragma once
#include "FairTask.h"
#include "R3BIOConnector.h"
#include "R3BNeulandPoint.h"
#include <bitset>

enum class BitsetParticle : uint32_t
{
    proton = 0x0001,
    neutron = 0x0002,
    electron = 0x0004,
    positron = 0x0008,
    alpha = 0x0010,
    gamma = 0x0020
};

constexpr auto ParticleBitsetSize = 32U;

constexpr auto ParticleToBitset(BitsetParticle particle);
auto BitsetToParticle(std::bitset<ParticleBitsetSize> bits) -> BitsetParticle;
auto CheckCriteria(BitsetParticle particle, BitsetParticle criteria) -> bool;

auto operator|(BitsetParticle left, BitsetParticle right) -> BitsetParticle;
auto operator&(BitsetParticle left, BitsetParticle right) -> BitsetParticle;
auto operator~(BitsetParticle particle) -> BitsetParticle;

enum class FilteringMode
{
    none,
    all_but_protons,
    custom
};
enum class FilteringMethod
{
    include,
    exclude
};

inline auto StringToFilteringMode(const std::string& input_string) -> FilteringMode
{

    if (input_string == "none")
    {
        return FilteringMode::none;
    }
    if (input_string == "all-but-protons")
    {
        return FilteringMode::all_but_protons;
    }
    if (input_string == "custom")
    {
        return FilteringMode::custom;
    }

    throw R3B::logic_error("invalid pid-filter string");
}

class NeulandPointFilter : public FairTask
{
  public:
    NeulandPointFilter() = default;

    void SetFilter(FilteringMode filtering_mode);

  private:
    auto Init() -> InitStatus override;
    void Exec(Option_t* /*option*/) override;

    static auto check_pid_for_criteria(int pid, BitsetParticle criteria) -> bool;

    static void apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                             R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                             BitsetParticle AllowedParticles);

    FilteringMode filtering_mode_ = FilteringMode::none;

    R3B::InputVectorConnector<R3BNeulandPoint> neuland_points_{ "NeulandPoints" };
    R3B::OutputVectorConnector<R3BNeulandPoint> filtered_neuland_points_{ "FilteredNeulandPoints" };
};
