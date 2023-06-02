#pragma once
#include "R3BNeulandYamlConfig.h"

class FairModule;
class R3BNeuland;
class R3BCave;

namespace R3B::yml
{
    template <typename Det>
    auto CreateDetWithYaml(const R3B::yml::Manager& /*manager*/) -> std::unique_ptr<FairModule>
    {
        std::cerr << "Warning: Creation of the detector from the YAML config is not implemented!" << std::endl;
        return nullptr;
    }
    template <>
    auto CreateDetWithYaml<R3BNeuland>(const R3B::yml::Manager& manager) -> std::unique_ptr<FairModule>;
    template <>
    auto CreateDetWithYaml<R3BCave>(const R3B::yml::Manager& manager) -> std::unique_ptr<FairModule>;

    namespace Cave
    {

        struct Simulation
        {
            DNode<bool> status = { "status" };
            DNode<std::string> geo_file = { "geo-file" };
        };

        struct YamlConfig
        {
            DNode<Simulation> simulation = { "simulation" };
        };

    } // namespace Cave

} // namespace R3B::yml
