#include "R3BYamlDetectors.h"
#include <R3BCave.h>
#include <R3BNeuland.h>

namespace R3B::yml
{

    template <>
    auto CreateDetWithYaml<R3BNeuland>(const R3B::yml::Manager& manager) -> std::unique_ptr<FairModule>
    {
        DNode<Neuland::YamlConfig> neulandConfig = { "neuland" };
        manager.Parse(neulandConfig);
        if (neulandConfig().simulation().status())
        {
            auto const nDP = neulandConfig().simulation().nDP();
            const auto& pos = neulandConfig().simulation().position();
            auto const neulandGeoTrans = TGeoTranslation{ pos[0], pos[1], pos[2] };
            return std::make_unique<R3BNeuland>(nDP, neulandGeoTrans);
        }
        return nullptr;
    }

    template <>
    auto CreateDetWithYaml<R3BCave>(const R3B::yml::Manager& manager) -> std::unique_ptr<FairModule>
    {
        DNode<Cave::YamlConfig> caveConfig = { "cave" };
        manager.Parse(caveConfig);

        if (caveConfig().simulation().status())
        {
            auto cave = std::make_unique<R3BCave>("CAVE");
            cave->SetGeometryFileName(caveConfig().simulation().geo_file());
            return cave;
        }
        return nullptr;
    }
} // namespace R3B::yml
