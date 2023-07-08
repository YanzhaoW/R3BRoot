#include "R3BEventHeader.h"
#include "R3BEventHeaderPropagator.h"
#include "R3BFileSource2.h"
#include "R3BNeulandHitPar.h"
#include "R3BNeulandMapped2CalPar.h"
#include "R3BTCalContFact.h"
#include <FairLogger.h>
#include <FairParRootFileIo.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>
#include <R3BProgramOptions.h>
#include <R3BTCalPar.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TStopwatch.h>
#include <fmt/core.h>
#include <iostream>

using namespace std::string_literals;
auto main(int argc, const char** argv) -> int
{
    auto timer = TStopwatch{};
    timer.Start();

    auto programOptions = R3B::ProgramOptions("options for neuland exp data analysis");
    auto help = programOptions.Create_Option<bool>("help,h", "help message", false);
    auto logLevel = programOptions.Create_Option<std::string>("logLevel,v", "set log level of fairlog", "error");

    if (!programOptions.Verify(argc, argv))
    {
        return EXIT_FAILURE;
    }

    if (help->value())
    {
        std::cout << programOptions.Get_DescRef() << std::endl;
        return 0;
    }
    FairLogger::GetLogger()->SetLogScreenLevel(logLevel->value().c_str());

    auto const fileID = 159;

    auto const fileNum = 10;

    // auto const calParFolder = std::string{ "/lustre/r3b/202205_s509/NeuLAND_CalData" };

    // auto const calParFile =
    //     fmt::format("/lustre/r3b/202205_s509/NeuLAND_MapData/Parameters/params_tcal_{0:04d}.root", fileID);

    auto const calParFile = "calPar.root"s;

    // auto const hitParFile =
    //     fmt::format("/lustre/r3b/202205_s509/NeuLAND_CalData/Parameters/params_sync_{0:04d}.root", fileID);

    auto const inputMapFilePattern = std::string{ "/lustre/r3b/202205_s509/NeuLAND_MapData/map{0:04d}.root" };

    auto const outputFileName = fmt::format("cal{0:04d}.root", fileID);

    const Int_t nBarsPerPlane = 50; // number of scintillator bars per plane
    const Int_t nPlanes = 26;       // number of planes (for TCAL calibration)
    const Int_t nev = -1;
    const Int_t trigger = -1;    // 1 - onspill, 2 - offspill. -1 - all
    const Int_t cosmtpat = 8192; // cosmics trigger : off-spill for NeuLAND -> 2^(14-1)

    auto source = std::make_unique<R3BFileSource2>();
    source->SetRunId(999);

    auto sinkFile = R3B::make_rootfile(outputFileName.c_str(), "RECREATE");
    auto sink = std::make_unique<FairRootFileSink>(sinkFile.release());

    for (auto i = 0; i < fileNum; ++i)
    {
        source->AddFile(fmt::format(inputMapFilePattern, fileID + i));
    }

    auto run = std::make_unique<FairRunAna>();
    FairRootManager::SetTreeName("evt");

    auto EvntHeader = std::make_unique<R3BEventHeader>();
    run->SetEventHeader(EvntHeader.release());
    run->SetSource(source.release());
    run->SetSink(sink.release());

    auto inputPars = std::make_unique<TList>();
    auto parLists = std::vector<std::unique_ptr<TObjString>>{};
    // parLists.push_back(std::make_unique<TObjString>((calParFolder + "/neuland_mapping_s509.root").c_str()));
    // parLists.push_back(std::make_unique<TObjString>((calParFolder + "/neuland_mapping_s509.root").c_str()));
    parLists.push_back(std::make_unique<TObjString>(calParFile.c_str()));

    for (auto& par : parLists)
    {
        inputPars->Add(par.release());
    }

    auto parOut = std::make_unique<FairParRootFileIo>(true);
    parOut->open(calParFile.c_str());

    // rtdb->getContainer("LandTCalPar");
    // auto res = rtdb->addContainer(tCalPar.release());
    // if (!res)
    // {
    //     throw std::runtime_error("container not created");
    // }
    // rtdb->getContainer("neulandMappingPar");
    // rtdb->setInputVersion(runID, const_cast<char*>("neulandMappingPar"), 1, 1);
    // rtdb->setInputVersion(runID, const_cast<char*>("LandTCalPar"), 1, 1);

    // auto* neulandHitPar = dynamic_cast<R3BNeulandHitPar*>(rtdb->getContainer("NeulandHitPar"));
    // neulandHitPar->SetNumberOfPlanes(nPlanes);

    // Add analysis task --------------------------------------------------------
    auto runIdTask = std::make_unique<R3BEventHeaderPropagator>();
    run->AddTask(runIdTask.release());

    const int minStats = 1000;
    auto neulandMapParameter = std::make_unique<R3BNeulandMapped2CalPar>();
    neulandMapParameter->SetMinStats(minStats);
    neulandMapParameter->SetTrigger(trigger);
    neulandMapParameter->SetNofModules(nPlanes, nBarsPerPlane);
    run->AddTask(neulandMapParameter.release());

    auto* rtdb = run->GetRuntimeDb();
    auto tCalParFact = std::make_unique<R3BTCalContFact>();
    rtdb->addContFactory(tCalParFact.release());

    rtdb->setOutput(parOut.release());
    rtdb->saveOutput();

    run->Init();
    run->Run(1000000);

    timer.Stop();
    auto* sinkfile = run->GetSink();
    if (sinkfile != nullptr)
    {
        sinkfile->Close();
    }
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;

    return 0;
}
