#include "NeulandPointFilter.h"
#include <iostream>
#include <utility>

void NeulandPointFilter::AddInclusionPDG(int inclusion_pdg)
{
    included_pdgs_hash_.emplace(inclusion_pdg, inclusion_pdg);
}
void NeulandPointFilter::AddExclusionPDG(int exclusion_pdg)
{
    excluded_pdgs_hash_.emplace(exclusion_pdg, exclusion_pdg);
}
void NeulandPointFilter::ClearInclusionPDGs() { included_pdgs_hash_.clear(); }
void NeulandPointFilter::ClearExclusionPDGs() { excluded_pdgs_hash_.clear(); }

void NeulandPointFilter::SetFilter(std::string filtering_mode) { filtering_mode_ = std::move(filtering_mode); }

void NeulandPointFilter::ApplyFilter()
{
    for (const auto& neuland_point : neuland_points_.get())
    {
        if ((included_pdgs_hash_.find(neuland_point.GetPID()) != included_pdgs_hash_.end()) &&
            (excluded_pdgs_hash_.find(neuland_point.GetPID()) == excluded_pdgs_hash_.end()))
        {
            filtered_neuland_points_.get().emplace_back(neuland_point);
        }
    }
}
auto NeulandPointFilter::Init() -> InitStatus
{
    neuland_points_.init();
    filtered_neuland_points_.init();

    return kSUCCESS;
}

void NeulandPointFilter::ProtonOnlyFilter()
{
    filtered_neuland_points_.clear();
    // Reset ex- and inclusion lists
    ClearInclusionPDGs();
    ClearExclusionPDGs();
    // Setup desired filters
    const auto proton_pdg = 2212;
    AddInclusionPDG(proton_pdg);
    // Apply filters
    ApplyFilter();
}

void NeulandPointFilter::CustomFilter()
{
    filtered_neuland_points_.clear();
    // Reset ex- and inclusion lists
    ClearInclusionPDGs();
    ClearExclusionPDGs();
    // Setup desired filters - currently filtering for anything that isnt a proton
    const auto proton_pdg = 2212;
    AddExclusionPDG(proton_pdg);
    // Apply filters
    ApplyFilter();
}

void NeulandPointFilter::Exec(Option_t* /*option*/)
{
    if (filtering_mode_ == "protons-only")
    {
        ProtonOnlyFilter();
    }
    if (filtering_mode_ == "custom")
    {
        CustomFilter();
    }
}
