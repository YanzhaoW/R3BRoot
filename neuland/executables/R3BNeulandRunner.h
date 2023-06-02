#pragma once

#include <R3BYamlConfigGeneral.h>
namespace R3B::Neuland
{
    class Runner
    {
      public:
        Runner() = default;
        void InitConfig(const R3B::yml::Manager& manager);
        [[nodiscard]] auto GetGeneralConfig() const -> const auto& { return general_; }
        void SetupEnv();

      private:
        yml::DNode<yml::General> general_ = { "general" };
    };
} // namespace R3B::Neuland
