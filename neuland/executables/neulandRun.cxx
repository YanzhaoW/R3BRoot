#include "FairBoxGenerator.h"
#include "FairParRootFileIo.h"
#include "FairPrimaryGenerator.h"
#include "FairRootFileSink.h"
#include "FairRunSim.h"
#include "R3BCave.h"
#include "R3BNeuland.h"
#include "R3BNeulandAnalyser.h"
#include "R3BNeulandSimulator.h"
#include "R3BShared.h"
#include "R3BYamlDetectors.h"
#include "R3BYamlTasks.h"
#include "TStopwatch.h"
#include <FairConstField.h>
#include <FairRunAna.h>
#include <FairTask.h>
#include <G4RunManager.hh>
#include <G4UserEventAction.hh>
#include <R3BIOFiles.h>
#include <R3BProgramOptions.h>
#include <TG4EventAction.h>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

using R3B::yml::CreateDetWithYaml;
using R3B::yml::CreateTaskWithYaml;
namespace fs = std::filesystem;

void RunOnline(const R3B::yml::Manager& ymlMan);
void RunAnalysis(const R3B::yml::Manager& ymlMan);
void RunSimulation(const R3B::yml::Manager& ymlMan, std::string suffix);

enum class Mode
{
    sim,
    ana,
    online
};

const auto ModeMap =
    std::unordered_map<std::string, Mode>{ { "sim", Mode::sim }, { "ana", Mode::ana }, { "online", Mode::online } };

int main(int argc, const char** argv) // NOLINT
{
    auto timer = TStopwatch{};
    timer.Start();

    auto programOptions = r3b::ProgramOptions("options for neuland simulation");
    auto help = programOptions.Create_Option<bool>("help,h", "help message", false);
    auto ymlFile = programOptions.Create_Option<std::string>("yml", "set the file path of the yml config file", "");
    auto mode =
        programOptions.Create_Option<std::string>("mode,m", R"(set the mode among "sim","ana","online")", "sim");

    if (!programOptions.Verify(argc, argv))
    {
        return EXIT_FAILURE;
    }

    if (help->value())
    {
        std::cout << programOptions.Get_DescRef() << std::endl;
        return 0;
    }
    const auto source_directory = fs::path{ gSystem->Getenv("VMCWORKDIR") };
    if (source_directory.empty())
    {
        throw std::runtime_error(
            "VMCWORKDIR is not defined! Please source the \"config.sh\" in the R3BRoot build directory.");
    }

    auto ymlMan = R3B::yml::Manager();
    ymlMan.AddFile(source_directory / "config/default.yml");
    ymlMan.AddFile(ymlFile->value());
    R3B::yml::DNode<R3B::yml::General> general = { "general" };
    ymlMan.Parse(general);

    auto ioFiles = R3B::yml::R3BIOManager{};
    ioFiles.SetInputParRegex(general().inputfile().para());
    ioFiles.SetInputRootRegex(general().inputfile().ana());
    ioFiles.SetOutputForamt(general().outputfile().ana());
    ioFiles.GetFiles();

    auto multiProcess = R3B::yml::MultiProc();
    multiProcess.SetMaxNTread(general().multi_process().max());

    auto* indexMan = R3B::yml::GetGlobalIndexManager();
    indexMan->Create_packs();
    indexMan->Set_suffix_len(general().multi_process().suffix_len());
    const auto& packs = indexMan->Get_packs();

    // for (const auto& pack : packs)
    // {
    //     auto const runSim = [&, pack]()
    //     {
    //         R3B::yml::SetGlobalIndices(pack);
    //         auto simulator = R3B::Neuland::Simulator{};
    //         auto suffix = indexMan->GetSuffix(pack);
    //         simulator.SetOutput_suffix(std::move(suffix));
    //         simulator.InitConfig(ymlMan);

    //         simulator.AddModule(CreateDetWithYaml<R3BCave>(ymlMan));
    //         simulator.AddModule(CreateDetWithYaml<R3BNeuland>(ymlMan));

    //         simulator.Run();
    //     };

    //     // switch (ModeMap.at(mode->value()))
    //     // {
    //     //     case Mode::sim:
    //     //         multiProcess.AddProcess(runSim);
    //     //         break;
    //     //     case Mode::ana:
    //     //         RunAnalysis(ymlMan);
    //     //         break;
    //     //     case Mode::online:
    //     //         RunOnline(ymlMan);
    //     //         break;
    //     // }
    // }

    multiProcess.Run();
    if (getpid() != multiProcess.GetMotherPID())
    {
        return 0;
    }

    timer.Stop();
    std::cout << "Executable finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
    return 0;
}

void RunSimulation(const R3B::yml::Manager& ymlMan, std::string suffix)
{
    auto simulator = R3B::Neuland::Simulator{};
    simulator.SetOutput_suffix(std::move(suffix));
    simulator.InitConfig(ymlMan);

    simulator.AddModule(CreateDetWithYaml<R3BCave>(ymlMan));
    simulator.AddModule(CreateDetWithYaml<R3BNeuland>(ymlMan));

    simulator.Run();
}
void RunOnline(const R3B::yml::Manager& ymlMan) {}
void RunAnalysis(const R3B::yml::Manager& ymlMan)
{
    auto analyser = R3B::Neuland::Analyser{};

    analyser.InitConfig(ymlMan);
    analyser.AddTask(CreateTaskWithYaml<R3BNeulandDigitizer>(ymlMan));
    analyser.AddTask(CreateTaskWithYaml<R3BNeulandHitMon>(ymlMan));

    analyser.Run();
}
