#pragma once
#include "R3BNeulandYamlConfig.h"
#include "R3BYamlConfig.h"

class FairTask;
class R3BNeulandDigitizer;
class R3BNeulandHitMon;

namespace R3B::yml
{

    template <typename Task>
    auto CreateTaskWithYaml(const R3B::yml::Manager& /*manager*/) -> std::unique_ptr<FairTask>
    {
        std::cerr << "Warning: Creation of the Task from the YAML config is not implemented!" << std::endl;
        return nullptr;
    }
    template <>
    auto CreateTaskWithYaml<R3BNeulandDigitizer>(const R3B::yml::Manager& /*manager*/) -> std::unique_ptr<FairTask>;
    template <>
    auto CreateTaskWithYaml<R3BNeulandHitMon>(const R3B::yml::Manager& /*manager*/) -> std::unique_ptr<FairTask>;

} // namespace R3B::yml
