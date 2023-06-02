#include "R3BNeulandAnalyser.h"
#include <FairFileSource.h>
#include <FairParRootFileIo.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <FairTask.h>

namespace R3B::Neuland
{
    Analyser::Analyser()
        : run_{ std::make_unique<FairRunAna>() }
    {
    }

    void Analyser::InitRun(FairRunAna* run)
    {
        PreInit(run);
        run->Init();
    }

    void Analyser::PreInit(FairRunAna* run)
    {
        const auto& general = GetGeneralConfig();
        auto filesource = std::make_unique<FairFileSource>(general().inputfile().ana().c_str());
        auto filesink = std::make_unique<FairRootFileSink>(general().outputfile().ana().c_str());
        run->SetSource(filesource.release());
        run->SetSink(filesink.release());
        SetupPar(run);
        SetupTasks(run);
    }
    void Analyser::PostRun(FairRunAna* run)
    {

        auto* sink = run->GetSink();
        sink->Close();
    }

    void Analyser::SetupPar(FairRunAna* run)
    {
        const auto& general = GetGeneralConfig();
        auto fileio = std::make_unique<FairParRootFileIo>();
        fileio->open(general().inputfile().para().c_str());
        run->GetRuntimeDb()->setFirstInput(fileio.release());
    }
    void Analyser::Run()
    {
        InitRun(run_.get());

        const auto& general = GetGeneralConfig();
        const auto& event = general().analysis().event();
        run_->Run(event[0], event[1]);
        PostRun(run_.get());
    }
    void Analyser::SetupTasks(FairRunAna* run)
    {

        for (auto& task : tasks_)
        {
            run->AddTask(task.release());
        }
    }

    void Analyser::AddTask(std::unique_ptr<FairTask> task)
    {

        if (task != nullptr)
        {
            tasks_.push_back(std::move(task));
        }
    }

} // namespace R3B::Neuland
