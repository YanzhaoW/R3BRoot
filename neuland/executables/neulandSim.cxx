#include "CosmicMuon.h"
#include "CosmicMuonDistributions.h"
#include "FairParRootFileIo.h"
#include "FairRootFileSink.h"
#include "FairRunSim.h"
#include "Generators.h"
#include "R3BCave.h"
#include "R3BNeuland.h"
#include "TStopwatch.h"
#include <G4RunManager.hh>
#include <G4UserEventAction.hh>
#include <R3BFieldConst.h>
#include <R3BPhaseSpaceGenerator.h>
#include <R3BProgramOptions.h>
#include <TG4EventAction.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>
#include <ctime>
#include <iostream>
#include <string>

inline constexpr auto DEFAULT_RUNID = 999;
inline constexpr auto defaultEventNum = 10;

auto main(int argc, const char** argv) -> int
{
    auto timer = TStopwatch{};
    timer.Start();

    auto programOptions = R3B::ProgramOptions("options for neuland simulation");

    auto help = programOptions.create_option<bool>("help,h", "help message", false);
    auto eventNum = programOptions.create_option<int>("eventNum", "set total event number", defaultEventNum);
    auto eventPrintNum = programOptions.create_option<int>("eventPrint", "set event print number", 1);
    auto runID = programOptions.create_option<int>("runID", "set runID", DEFAULT_RUNID);
    auto multi = programOptions.create_option<int>("multiplicity", "set particle multiplicity", 1);
    auto pEnergy = programOptions.create_option<double>("energy", "set energy value (GeV) of the particle", 1);
    auto simuFileName =
        programOptions.create_option<std::string>("simuFile", "set the base filename of simulation ouput", "simu.root");
    auto generator = programOptions.create_option<std::string>("gen,G", "Set the generator types: muon, box", "box");
    auto paraFileName =
        programOptions.create_option<std::string>("paraFile", "set the base filename of parameter sink", "para.root");
    auto logLevel = programOptions.create_option<std::string>("logLevel,v", "set log level of fairlog", "error");

    if (!programOptions.verify(argc, argv))
    {
        return EXIT_FAILURE;
    }

    if (help())
    {
        std::cout << programOptions.get_desc_ref() << std::endl;
        return 0;
    }

    // Logging
    // FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
    FairLogger::GetLogger()->SetLogScreenLevel(logLevel().c_str());

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

    // Basic simulation setup
    auto run = std::make_unique<FairRunSim>();
    run->SetName("TGeant4");
    run->SetRunId(runID());
    run->SetStoreTraj(false);
    run->SetMaterials("media_r3b.geo");
    run->SetSink(std::make_unique<FairRootFileSink>(simuFileName().c_str()));
    auto fairField = std::make_unique<R3BFieldConst>();
    run->SetField(fairField.release());

    // auto primGen = create_muon_generator();
    TRandom3 random_gen(0);
    auto primGen = [&random_gen, gen = generator.value(), multi = multi.value(), energy = pEnergy.value()]()
    {
        if (gen == "muon")
        {
            return create_muon_generator(random_gen);
        }
        if (gen == "box")
        {
            return create_box_generator(energy, multi);
        }
        throw std::runtime_error(fmt::format("unrecognized generator type: {}!", gen));
    }();

    run->SetGenerator(primGen.release());

    // Geometry: Cave
    auto cave = std::make_unique<R3BCave>("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave.release());

    // Geometry: Neuland
    // auto const nDP = 13;
    auto const neulandGeoTrans = TGeoTranslation{ 0., 0., z_pos };
    auto neuland = std::make_unique<R3BNeuland>(nDP, neulandGeoTrans);
    run->AddModule(neuland.release());

    // Init
    run->Init();

    // event print out:
    auto* grun = G4RunManager::GetRunManager();
    grun->SetPrintProgress(eventPrintNum());
    auto* event = dynamic_cast<TG4EventAction*>(const_cast<G4UserEventAction*>(grun->GetUserEventAction())); // NOLINT
    event->VerboseLevel(0);

    // Connect runtime parameter file
    auto parFileIO = std::make_unique<FairParRootFileIo>(true);
    parFileIO->open(paraFileName().c_str());
    auto* rtdb = run->GetRuntimeDb();
    rtdb->setOutput(parFileIO.release());
    rtdb->saveOutput();

    // Simulate
    run->Run(eventNum());

    // Report
    timer.Stop();
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
    return 0;
}
