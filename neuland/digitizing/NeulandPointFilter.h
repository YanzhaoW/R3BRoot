#pragma once
#include "FairTask.h"
#include "R3BIOConnector.h"
#include "R3BNeulandPoint.h"

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

    static void add_filtered_pid(int filtered_pid, std::unordered_map<int, int>& filtered_pids_hash);

    static void clear_filtered_pids_hash(std::unordered_map<int, int>& filtered_pids_hash);

    static void apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                             R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                             FilteringMethod filtering_method,
                             std::unordered_map<int, int>& filtered_pids_hash);

    std::unordered_map<int, int> filtered_pids_hash_;

    FilteringMode filtering_mode_ = FilteringMode::none;

    R3B::InputVectorConnector<R3BNeulandPoint> neuland_points_{ "NeulandPoints" };
    R3B::OutputVectorConnector<R3BNeulandPoint> filtered_neuland_points_{ "FilteredNeulandPoints" };
};
