#include "DigitizingChannelMock.h"
#include "DigitizingPaddleMock.h"
#include "DigitizingPaddleNeuland.h"
#include "DigitizingTacQuila.h"
#include "DigitizingTamex.h"
#include "FairFileSource.h"
#include "FairParRootFileIo.h"
#include "FairRootFileSink.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BNeulandDigitizer.h"
#include "R3BNeulandHitMon.h"
#include "R3BProgramOptions.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <boost/program_options.hpp>

using std::cout;
using std::endl;

using NeulandPaddle = Digitizing::Neuland::NeulandPaddle;
using MockPaddle = Digitizing::Neuland::MockPaddle;
using TamexChannel = Digitizing::Neuland::Tamex::Channel;
using TacquilaChannel = Digitizing::Neuland::TacQuila::Channel;
using MockChannel = Digitizing::Neuland::MockChannel;
using Digitizing::UseChannel;
using Digitizing::UsePaddle;

const auto neulandEngines = std::map<std::pair<const std::string, const std::string>,
                                     std::function<std::unique_ptr<Digitizing::DigitizingEngineInterface>()>>{
    { { "neuland", "tamex" },
      []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TamexChannel>()); } },
    { { "neuland", "tacquila" },
      []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TacquilaChannel>()); } },
    { { "mock", "tamex" },
      []() { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<TamexChannel>()); } },
    { { "neuland", "mock" },
      []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<MockChannel>()); } },
    { { "mock", "mock" },
      []() { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<MockChannel>()); } }
};

int main(int argc, const char** argv)
{
    TStopwatch timer;
    timer.Start();

    auto programOptions = r3b::ProgramOptions("options for neuland data analysis");
    auto help = programOptions.Create_Option<bool>("help,h", "help message", false);
    auto paddleName =
        programOptions.Create_Option<std::string>("paddle", R"(set the paddle name. e.g. "neuland")", "neuland");
    auto channelName =
        programOptions.Create_Option<std::string>("channel", R"(set the channel name. e.g. "tamex")", "tacquila");
    auto simuFileName =
        programOptions.Create_Option<std::string>("simuFile", "set the filename of simulation input", "simu.root");
    auto paraFileName =
        programOptions.Create_Option<std::string>("paraFile", "set the filename of parameter sink", "para.root");
    auto digiFileName =
        programOptions.Create_Option<std::string>("digiFile", "set the filename of digitization output", "digi.root");
    auto logLevel = programOptions.Create_Option<std::string>("logLevel,v", "set log level of fairlog", "error");
    auto eventNum = programOptions.Create_Option<int>("eventNum,n", "set total event number", 0);

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

    auto run = std::make_unique<FairRunAna>();
    auto filesource = std::make_unique<FairFileSource>(simuFileName->value().c_str());
    auto filesink = std::make_unique<FairRootFileSink>(digiFileName->value().c_str());
    run->SetSource(filesource.release());
    run->SetSink(filesink.release());

    auto fileio = std::make_unique<FairParRootFileIo>();
    fileio->open(paraFileName->value().c_str());
    run->GetRuntimeDb()->setFirstInput(fileio.get());

    auto digiNeuland = std::make_unique<R3BNeulandDigitizer>();
    digiNeuland->SetPaddleChannel((neulandEngines.at({ paddleName->value(), channelName->value() }))());
    // digiNeuland->SetPaddleChannel(UsePaddle<NeulandPaddle>(),
    //                               UseChannel<TamexChannel>(digiNeuland->GetNeulandHitParRef()));
    run->AddTask(digiNeuland.get());
    auto hitmon = std::make_unique<R3BNeulandHitMon>();
    run->AddTask(hitmon.get());

    run->Init();
    run->Run(0, eventNum->value());

    timer.Stop();
    auto* sink = run->GetSink();
    sink->Close();
    cout << "Macro finished successfully." << endl;
    cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
}
