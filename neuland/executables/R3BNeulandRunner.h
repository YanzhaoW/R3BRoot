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
        void SetOutput_suffix(std::string suffix) { output_suffix_ = std::move(suffix); }
        void SetOutput_prefix(std::string prefix) { output_prefix_ = std::move(prefix); }
        auto GetOutputStr(const std::string& basename) -> std::string;
        void SetInputFile(std::string inputfile) { input_file_ = std::move(inputfile); }
        void SetOutputFile(std::string outputfile) { output_file_ = std::move(outputfile); }

      private:
        yml::DNode<yml::General> general_ = { "general" };
        std::string output_suffix_;
        std::string output_prefix_;
        std::string input_file_;
        std::string output_file_;
    };
} // namespace R3B::Neuland
