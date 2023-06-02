#pragma once

#include "R3BNeulandRunner.h"

class FairRunAna;
class FairTask;

namespace R3B::Neuland
{

    class Analyser : public Runner
    {
      public:
        Analyser();
        void AddTask(std::unique_ptr<FairTask> task);
        void Run();

      private:
        std::unique_ptr<FairRunAna> run_;
        std::vector<std::unique_ptr<FairTask>> tasks_;
        void InitRun(FairRunAna* run);
        void PreInit(FairRunAna* run);
        static void PostRun(FairRunAna* run);
        void SetupPar(FairRunAna* run);
        void SetupTasks(FairRunAna* run);
    };
} // namespace R3B::Neuland
