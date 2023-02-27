#include "FairBoxGenerator.h"
#include "FairParRootFileIo.h"
#include "FairPrimaryGenerator.h"
#include "FairRootFileSink.h"
#include "FairRunSim.h"
#include "R3BCave.h"
#include "R3BNeuland.h"
#include "TStopwatch.h"
#include <FairConstField.h>
#include <G4RunManager.hh>
#include <G4UserEventAction.hh>
#include <TG4EventAction.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int argc, const char** argv)
{
    TStopwatch timer;
    auto eventNum = 100000;
    auto eventPrintNum = 1000;
    auto multi = 1;
    auto PID = 2112;
    auto pEnergy = 1.;
    auto simuFileName = std::string{ "simu.root" };
    auto paraFileName = std::string{ "para.root" };
    auto logLevel = std::string{ "error" };
    timer.Start();

    auto desc = po::options_description{ "options for neuland simulation" };

    desc.add_options()("help", "help message")("eventNum", po::value<int>(), "set total event number")(
        "eventPrint", po::value<int>(), "set event print number")("PID", po::value<int>(), "set particle id")(
        "energy", po::value<double>(), "set energy value (GeV) of the particle")(
        "multiplicity", po::value<int>(), "set particle multiplicity")(
        "simuFile", po::value<std::string>(), "set the base filename (no .root) of simulation ouput")(
        "paraFile", po::value<std::string>(), "set the base filename (no .root) of parameter sink")(
        "logLevel", po::value<std::string>(), "set log level of fairlog");

    auto varMap = po::variables_map{};
    po::store(po::parse_command_line(argc, argv, desc), varMap);
    po::notify(varMap);

    if (varMap.count("help") != 0U)
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (varMap.count("eventNum") != 0U)
    {
        eventNum = varMap["eventNum"].as<int>();
    }
    if (varMap.count("eventPrint") != 0U)
    {
        eventPrintNum = varMap["eventPrint"].as<int>();
    }
    if (varMap.count("multiplicity") != 0U)
    {
        multi = varMap["multiplicity"].as<int>();
    }
    if (varMap.count("PID") != 0U)
    {
        PID = varMap["PID"].as<int>();
    }
    if (varMap.count("energy") != 0U)
    {
        pEnergy = varMap["energy"].as<double>();
    }
    if (varMap.count("simuFile") != 0U)
    {
        simuFileName = std::string{ "simu_" } + varMap["simuFile"].as<std::string>();
    }
    if (varMap.count("paraFile") != 0U)
    {
        paraFileName = std::string{ "para_" } + varMap["paraFile"].as<std::string>();
    }
    if (varMap.count("logLevel") != 0U)
    {
        logLevel = varMap["logLevel"].as<std::string>();
    }

    // Logging
    FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
    FairLogger::GetLogger()->SetLogScreenLevel(logLevel.c_str());

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

    // Basic simulation setup
    auto run = new FairRunSim();
    run->SetName("TGeant4");
    run->SetStoreTraj(false);
    run->SetMaterials("media_r3b.geo");
    run->SetSink(new FairRootFileSink(simuFileName.c_str()));
    run->SetField(new FairConstField());

    // Primary particle generator
    auto boxGen = new FairBoxGenerator(PID, multi);
    boxGen->SetXYZ(0, 0, 0.);
    boxGen->SetThetaRange(0., 3.);
    boxGen->SetPhiRange(0., 360.);
    boxGen->SetEkinRange(pEnergy, pEnergy);
    auto primGen = new FairPrimaryGenerator();
    primGen->AddGenerator(boxGen);
    run->SetGenerator(primGen);

    // Geometry: Cave
    auto cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    // Geometry: Neuland
    run->AddModule(new R3BNeuland(30, { 0., 0., 1650. }));

    // Init
    run->Init();

    // event print out:
    auto grun = G4RunManager::GetRunManager();
    grun->SetPrintProgress(eventPrintNum);
    auto event = dynamic_cast<TG4EventAction*>(const_cast<G4UserEventAction*>(grun->GetUserEventAction()));
    event->VerboseLevel(0);

    // Connect runtime parameter file
    auto parFileIO = new FairParRootFileIo(true);
    parFileIO->open(paraFileName.c_str());
    auto rtdb = run->GetRuntimeDb();
    rtdb->setOutput(parFileIO);
    rtdb->saveOutput();

    // Simulate
    run->Run(eventNum);

    // Report
    timer.Stop();
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
    return 0;
}
