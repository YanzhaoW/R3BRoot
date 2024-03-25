#include "NeulandPointFilter.h"
#include <iostream>
#include <utility>

constexpr auto proton_pid = 2212;

void NeulandPointFilter::add_filtered_pid(int filtered_pid, std::unordered_map<int, int>& filtered_pids_hash)
{
    filtered_pids_hash.emplace(filtered_pid, filtered_pid);
}
void NeulandPointFilter::clear_filtered_pids_hash(std::unordered_map<int, int>& filtered_pids_hash)
{
    filtered_pids_hash.clear();
}
void NeulandPointFilter::SetFilter(FilteringMode filtering_mode) { filtering_mode_ = filtering_mode; }

void NeulandPointFilter::apply_filter(const R3B::InputVectorConnector<R3BNeulandPoint>& neuland_points,
                                      R3B::OutputVectorConnector<R3BNeulandPoint>& filtered_neuland_points,
                                      FilteringMethod filtering_method,
                                      std::unordered_map<int, int>& filtered_pids_hash)
{
    switch (filtering_method)
    {
        case FilteringMethod::include:
            for (const auto& neuland_point : neuland_points.get())
            {
                if (filtered_pids_hash.find(neuland_point.GetPID()) != filtered_pids_hash.end())
                {
                    filtered_neuland_points.get().emplace_back(neuland_point);
                }
            }
            break;
        case FilteringMethod::exclude:
            for (const auto& neuland_point : neuland_points.get())
            {
                if (filtered_pids_hash.find(neuland_point.GetPID()) == filtered_pids_hash.end())
                {
                    filtered_neuland_points.get().emplace_back(neuland_point);
                }
            }
            break;
        default:
            std::cout << "Unexpected FilteringMethod\n";
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
            clear_filtered_pids_hash(filtered_pids_hash_);
            add_filtered_pid(proton_pid, filtered_pids_hash_);
            apply_filter(neuland_points_, filtered_neuland_points_, FilteringMethod::include, filtered_pids_hash_);
            break;
        case FilteringMode::custom:
            clear_filtered_pids_hash(filtered_pids_hash_);
       //     add_filtered_pid(proton_pid, filtered_pids_hash_);
            apply_filter(neuland_points_, filtered_neuland_points_, FilteringMethod::include, filtered_pids_hash_);
            break;
        default:
            std::cout << "Unexpected FilteringMode\n";
    }
}
