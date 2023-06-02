#pragma once
#include "R3BNeulandRunner.h"
#include <FairModule.h>
#include <unordered_map>

class FairRunSim;
class FairPrimaryGenerator;

namespace R3B::Neuland
{
    extern const std::unordered_map<std::string, int> PDGID_map;

    class Simulator : public Runner
    {
      public:
        Simulator();
        void Run();
        static auto Get_PDG_ID(const std::string& name) -> int;

      private:
        std::unique_ptr<FairRunSim> run_;
        std::vector<std::unique_ptr<FairModule>> detectors_;
        void InitRun();
        void SetupParaOutput();
        void SetupModules(FairRunSim* run);
        void SetupEventPrint();
        void SetupRun(FairRunSim* run);
        void SetupGenerator(FairPrimaryGenerator* primGen);

      public:
        void AddModule(std::unique_ptr<FairModule> module)
        {
            if (module != nullptr)
            {
                detectors_.emplace_back(std::move(module));
            }
        }
    };

} // namespace R3B::Neuland
