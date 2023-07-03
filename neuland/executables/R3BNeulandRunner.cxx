#include "R3BNeulandRunner.h"
#include <FairLogger.h>
#include <TSystem.h>

namespace R3B::Neuland
{

    std::unordered_map<std::string, int> const PDGID_map = { { "neutron", 2112 },
                                                             { "proton", 2212 },
                                                             { "electron", 11 },
                                                             { "gamma", 22 },
                                                             { "alpha", 1000020040 } };

    // TODO: in base class
    void Runner::SetupEnv()
    {
        const auto source_directory = std::string{ gSystem->Getenv("VMCWORKDIR") };
        auto geoPath = source_directory + std::string{ "/geometry" };
        auto geantConfigDir = source_directory + std::string{ "/gconfig" };
        FairLogger::GetLogger()->SetLogScreenLevel(general_().log_level().c_str());
        gSystem->Setenv("VMCWORKDIR", source_directory.c_str());
        gSystem->Setenv("GEOMPATH", geoPath.c_str());
        gSystem->Setenv("CONFIG_DIR", geantConfigDir.c_str());
    }

    void Runner::InitConfig(const R3B::yml::Manager& manager)
    {
        manager.Parse(general_);
        SetupEnv();
    }

    auto Runner::GetOutputStr(const std::string& basename) -> std::string
    {
        if (auto const pos = basename.find(".root"); pos != std::string::npos)
        {
            auto temp = basename;
            temp.insert(pos, output_suffix_);
            temp.insert(0, output_prefix_);
            return temp;
        }
        throw std::runtime_error("Output file name must end with \".root\".");
    }
} // namespace R3B::Neuland
