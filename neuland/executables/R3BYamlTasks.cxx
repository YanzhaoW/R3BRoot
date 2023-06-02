#include "R3BYamlTasks.h"
#include "R3BDigitizingChannelMock.h"
#include "R3BDigitizingPaddleMock.h"
#include "R3BDigitizingPaddleNeuland.h"
#include "R3BDigitizingTacQuila.h"
#include "R3BDigitizingTamex.h"
#include "R3BNeulandDigitizer.h"
#include "R3BNeulandHitMon.h"
#include <map>

namespace
{
    namespace Digitizing = R3B::Digitizing;
    using NeulandPaddle = Digitizing::Neuland::NeulandPaddle;
    using MockPaddle = Digitizing::Neuland::MockPaddle;
    using TamexChannel = Digitizing::Neuland::Tamex::Channel;
    using TacquilaChannel = Digitizing::Neuland::TacQuila::Channel;
    using MockChannel = Digitizing::Neuland::MockChannel;
    using Digitizing::UseChannel;
    using Digitizing::UsePaddle;

} // namespace

namespace R3B::yml
{
    template <>
    auto CreateTaskWithYaml<R3BNeulandDigitizer>(const R3B::yml::Manager& manager) -> std::unique_ptr<FairTask>
    {
        DNode<Neuland::YamlConfig> config = { "neuland" };
        manager.Parse(config);
        const auto& digiConfig = config().analysis().digitizer();

        auto const channelInit = [hit_par = digiConfig.hit_par()]()
        { Digitizing::Neuland::Tamex::Channel::GetHitPar(hit_par); };

        const auto neulandEngines = std::map<std::pair<const std::string, const std::string>,
                                             std::function<std::unique_ptr<Digitizing::DigitizingEngineInterface>()>>{
            { { "neuland", "tamex" },
              [&]() {
                  return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TamexChannel>(), channelInit);
              } },
            { { "neuland", "tacquila" },
              []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TacquilaChannel>()); } },
            { { "mock", "tamex" },
              [&]()
              { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<TamexChannel>(), channelInit); } },
            { { "mock", "tacquila" },
              [&]() { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<TacquilaChannel>()); } },
            { { "neuland", "mock" },
              []() { return Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<MockChannel>()); } },
            { { "mock", "mock" },
              []() { return Digitizing::CreateEngine(UsePaddle<MockPaddle>(), UseChannel<MockChannel>()); } }
        };

        auto digiNeuland = std::make_unique<R3BNeulandDigitizer>();
        digiNeuland->SetEngine((neulandEngines.at({ digiConfig.paddle(), digiConfig.channel() }))());
        return digiNeuland;
    }

    template <>
    auto CreateTaskWithYaml<R3BNeulandHitMon>(const R3B::yml::Manager& /*manager*/) -> std::unique_ptr<FairTask>
    {
        return std::make_unique<R3BNeulandHitMon>();
    }

} // namespace R3B::yml
