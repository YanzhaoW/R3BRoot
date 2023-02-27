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

namespace po = boost::program_options;

int main(int argc, const char** argv)
{
    TStopwatch timer;
    timer.Start();

    auto channelName = std::string{ "tacquila" };
    auto paddleName = std::string{ "neuland" };
    auto paraFileName = std::string{ "para.root" };
    auto simuFileName = std::string{ "simu.root" };
    auto digiFileName = std::string{ "digi.root" };
    auto logLevel = std::string{ "error" };
    auto eventNum = 0;

    auto desc = po::options_description{ "options for neuland data analysis" };
    desc.add_options()("help", "help message")(
        "paddle", po::value<std::string>(), R"(set the paddle name. e.g. "neuland")")(
        "channel", po::value<std::string>(), R"(set the channel name. e.g. "tamex", "tacquila")")(
        "simuFile", po::value<std::string>(), "set the base filename of simulation input")(
        "digiFile", po::value<std::string>(), "set the base filename of digitization ouput")(
        "paraFile", po::value<std::string>(), "set the base filename of parameter input")(
        "eventN,n", po::value<int>(), "set the number of events")(
        "logLevel,v", po::value<std::string>(), "set log level of fairlog");

    auto varMap = po::variables_map{};
    po::store(po::parse_command_line(argc, argv, desc), varMap);
    po::notify(varMap);

    if (varMap.count("help") != 0U)
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (varMap.count("paddle") != 0U)
    {
        paddleName = varMap["paddle"].as<std::string>();
    }
    if (varMap.count("channel") != 0U)
    {
        channelName = varMap["channel"].as<std::string>();
    }
    if (varMap.count("simuFile") != 0U)
    {
        simuFileName = varMap["simuFile"].as<std::string>() + std::string{ ".root" };
    }
    if (varMap.count("digiFile") != 0U)
    {
        digiFileName = varMap["digiFile"].as<std::string>() + std::string{ ".root" };
    }
    if (varMap.count("paraFile") != 0U)
    {
        paraFileName = varMap["paraFile"].as<std::string>() + std::string{ ".root" };
    }
    if (varMap.count("logLevel") != 0U)
    {
        logLevel = varMap["logLevel"].as<std::string>();
    }
    if (varMap.count("eventN") != 0U)
    {
        eventNum = varMap["eventN"].as<int>();
    }
    FairLogger::GetLogger()->SetLogScreenLevel(logLevel.c_str());

    auto run = new FairRunAna{};
    run->SetSource(new FairFileSource(simuFileName.c_str()));
    run->SetSink(new FairRootFileSink(digiFileName.c_str()));

    auto io = new FairParRootFileIo();
    io->open(paraFileName.c_str());
    run->GetRuntimeDb()->setFirstInput(io);

    auto* digiNeuland = new R3BNeulandDigitizer();
    digiNeuland->SetPaddleChannel((neulandEngines.at({ paddleName, channelName }))());
    // digiNeuland->SetPaddleChannel(UsePaddle<NeulandPaddle>(),
    //                               UseChannel<TamexChannel>(digiNeuland->GetNeulandHitParRef()));
    run->AddTask(digiNeuland);
    run->AddTask(new R3BNeulandHitMon());

    run->Init();
    run->Run(0, eventNum);

    timer.Stop();
    auto sink = run->GetSink();
    sink->Close();
    cout << "Macro finished successfully." << endl;
    cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << endl;
}
