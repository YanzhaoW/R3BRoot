#include "FairFileSource.h"
#include "FairParRootFileIo.h"
#include "FairRootFileSink.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BDigitizingChannelMock.h"
#include "R3BDigitizingPaddleMock.h"
#include "R3BDigitizingPaddleNeuland.h"
#include "R3BDigitizingTacQuila.h"
#include "R3BDigitizingTamex.h"
#include "R3BFileSource2.h"
#include "R3BNeulandDigitizer.h"
#include "R3BNeulandHitMon.h"
#include "R3BNeulandPrimaryInteractionFinder.h"
#include "R3BProgramOptions.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <R3BNeulandClusterFinder.h>
#include <R3BNeulandMultiplicityCalorimetricTrain.h>
#include <R3BNeulandPrimaryClusterFinder.h>
#include <TObjString.h>
#include <boost/program_options.hpp>
#include <filesystem>

namespace fs = std::filesystem;
namespace Digitizing = R3B::Digitizing;
using NeulandPaddle = Digitizing::Neuland::NeulandPaddle;
using MockPaddle = Digitizing::Neuland::MockPaddle;
using TamexChannel = Digitizing::Neuland::Tamex::Channel;
using TacquilaChannel = Digitizing::Neuland::TacQuila::Channel;
using MockChannel = Digitizing::Neuland::MockChannel;
using Digitizing::UseChannel;
using Digitizing::UsePaddle;

inline auto ConcatPath(std::string_view dir_str, std::string_view filepath_str) -> fs::path
{
    auto dir = fs::path{ dir_str };
    auto filepath = fs::path{ filepath_str };
    return (filepath.is_absolute()) ? filepath : dir / filepath;
}

auto main(int argc, const char** argv) -> int
{
    auto timer = TStopwatch{};
    timer.Start();

    // ===================================================================
    // program options
    auto programOptions = R3B::ProgramOptions("options for neuland data analysis");
    auto help = programOptions.Create_Option<bool>("help,h", "help message", false);
    auto paddleName =
        programOptions.Create_Option<std::string>("paddle", R"(set the paddle name. e.g. "neuland")", "neuland");
    auto channelName =
        programOptions.Create_Option<std::string>("channel", R"(set the channel name. e.g. "tamex")", "tacquila");
    auto simuFileNames = programOptions.Create_Option<std::string>(
        "simu", "set semicolon separated filenames of simulation input", "simu.root");
    auto paraFileName =
        programOptions.Create_Option<std::string>("parIn", "set the filename of parameter sink", "para.root");
    auto paraFileName2 =
        programOptions.Create_Option<std::string>("parIn2", "set the filename of the second parameter sink", "");
    auto parOut = programOptions.Create_Option<std::string>(
        "parOut", "set the filename of the parameter output", "trainPar.root");
    auto digiFileName =
        programOptions.Create_Option<std::string>("digiFile", "set the filename of digitization output", "digi.root");
    auto inDir = programOptions.Create_Option<std::string>(
        "inDir", "set the file path of input directory (containing input tree data and parameter)", ".");
    auto outDir = programOptions.Create_Option<std::string>(
        "outDir", "set the file path of output directory (containing output tree data and parameter)", ".");
    auto logLevel = programOptions.Create_Option<std::string>("logLevel,v", "set log level of fairlog", "error");
    auto eventNum = programOptions.Create_Option<int>("eventNum,n", "set total event number", 0);
    auto hitLevelPar =
        programOptions.Create_Option<std::string>("hitLevelPar", "set the name of hit level parameter if needed.", "");

    if (!programOptions.Verify(argc, argv))
    {
        return EXIT_FAILURE;
    }

    if (help->value())
    {
        std::cout << programOptions.Get_DescRef() << std::endl;
        return 0;
    }

    // ===================================================================
    // variable definitions
    const auto outputFilePath = ConcatPath(outDir->value(), digiFileName->value());

    auto const channelInit = [&]()
    {
        if (not hitLevelPar->value().empty())
        {
            FairRuntimeDb::instance()->getContainer(hitLevelPar->value().c_str());
            Digitizing::Neuland::Tamex::Channel::GetHitPar(hitLevelPar->value());
        }
    };

    auto tamexParameter = Digitizing::Neuland::Tamex::Params{ TamexChannel::GetDefaultRandomGen() };
    tamexParameter.fPMTThresh = 1.;
    tamexParameter.fTimeMin = 1.;

    const auto neulandEngines = std::map<std::pair<const std::string, const std::string>,
                                         std::function<std::unique_ptr<Digitizing::DigitizingEngineInterface>()>>{
        { { "neuland", "tamex" },
          [&]()
          {
              return Digitizing::CreateEngine(
                  UsePaddle<NeulandPaddle>(), UseChannel<TamexChannel>(tamexParameter), channelInit);
          } },
        { { "neuland", "tacquila" },
          []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TacquilaChannel>()); } },
        { { "mock", "tamex" },
          [&]() {
              return Digitizing::CreateEngine(
                  UsePaddle<MockPaddle>(), UseChannel<TamexChannel>(tamexParameter), channelInit);
          } },
        { { "neuland", "mock" },
          []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<MockChannel>()); } },
        { { "mock", "mock" },
          []() { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<MockChannel>()); } }
    };

    FairLogger::GetLogger()->SetLogScreenLevel(logLevel->value().c_str());

    // ===================================================================
    // FairRun setup
    auto run = std::make_unique<FairRunAna>();
    auto filesource = std::make_unique<R3BFileSource2>();
    auto filename_begin = std::string::size_type{ 0 };
    for (auto filename_end = std::string::size_type{ 0 }; filename_end != std::string::npos;
         filename_begin = filename_end + 1)
    {
        filename_end = simuFileNames->value().find(';', filename_begin);
        auto filename = simuFileNames->value().substr(filename_begin, filename_end - filename_begin);
        auto filepath = ConcatPath(inDir->value(), filename);
        filesource->AddFile(filepath);
    }

    auto filesink = std::make_unique<FairRootFileSink>(outputFilePath.c_str());
    run->SetSource(filesource.release());
    run->SetSink(filesink.release());

    auto fileio = std::make_unique<FairParRootFileIo>();
    fileio->open((ConcatPath(inDir->value(), paraFileName->value())).c_str());
    run->GetRuntimeDb()->setFirstInput(fileio.release());

    if (const auto& filename = paraFileName2->value(); not filename.empty())
    {
        auto fileio2 = std::make_unique<FairParRootFileIo>();
        fileio2->open((ConcatPath(inDir->value(), paraFileName2->value())).c_str());
        run->GetRuntimeDb()->setSecondInput(fileio2.release());
    }

    // ===================================================================
    // adding tasks
    auto digiNeuland = std::make_unique<R3BNeulandDigitizer>();
    digiNeuland->SetEngine((neulandEngines.at({ paddleName->value(), channelName->value() }))());
    run->AddTask(digiNeuland.release());

    run->AddTask(std::make_unique<R3BNeulandPrimaryInteractionFinder>().release());
    run->AddTask(std::make_unique<R3BNeulandClusterFinder>().release());
    run->AddTask(std::make_unique<R3BNeulandClusterFinder>().release());
    run->AddTask(std::make_unique<R3BNeulandPrimaryClusterFinder>().release());
    run->AddTask(std::make_unique<R3BNeulandMultiplicityCalorimetricTrain>().release());

    auto hitmon = std::make_unique<R3BNeulandHitMon>();
    run->AddTask(hitmon.release());

    // ===================================================================
    // FairRun init
    run->Init();

    // ===================================================================
    // parameter output
    auto parFileIO = std::make_unique<FairParRootFileIo>(true);
    parFileIO->open((ConcatPath(inDir->value(), parOut->value())).c_str());
    auto* rtdb = run->GetRuntimeDb();
    rtdb->setOutput(parFileIO.release());
    rtdb->saveOutput();

    run->Run(0, eventNum->value());

    timer.Stop();
    auto* sink = run->GetSink();
    sink->Close();
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
}
