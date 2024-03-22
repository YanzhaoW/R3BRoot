#pragma once
#include "FairTask.h"
#include "R3BIOConnector.h"
#include "R3BNeulandPoint.h"

class NeulandPointFilter : public FairTask
{
  public:
    void SetFilter(std::string filtering_mode);

    void Exec(Option_t* /*option*/) override;

  protected:
    auto Init() -> InitStatus override;

  private:
    void AddInclusionPDG(int inclusion_pdg);
    void AddExclusionPDG(int exclusion_pdg);

    void ClearInclusionPDGs();
    void ClearExclusionPDGs();

    void ProtonOnlyFilter();
    void CustomFilter();

    void ApplyFilter();

    std::unordered_map<int, int> included_pdgs_hash_;
    std::unordered_map<int, int> excluded_pdgs_hash_;

    std::string filtering_mode_;

    R3B::InputVectorConnector<R3BNeulandPoint> neuland_points_{ "NeulandPoints" };
    R3B::OutputVectorConnector<R3BNeulandPoint> filtered_neuland_points_{ "NeulandPoints" };
};
