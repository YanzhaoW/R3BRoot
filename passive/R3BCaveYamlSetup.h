#pragma once

#include "R3BCave.h"
#include "R3BYamlConfig.h"
#include <string>

namespace R3B::yml::Cave
{

    class Configurator
    {
      public:
        explicit Configurator(const R3B::yml::Manager& manager) { manager.Parse(caveConfig); }

        [[nodiscard]] auto CreateModule() const -> std::unique_ptr<R3BCave>
        {

            auto cave = std::make_unique<R3BCave>("CAVE");
            cave->SetGeometryFileName(caveConfig().simulation().geo_file());
            return cave;
        }

      private:
        DNode<CaveYaml> caveConfig = { "cave" };
    };

} // namespace R3B::yml::Cave
