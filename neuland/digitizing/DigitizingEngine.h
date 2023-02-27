/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2023 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#ifndef NEULAND_DIGITIZING_ENGINE_H
#define NEULAND_DIGITIZING_ENGINE_H

#include "DigitizingChannel.h"
#include "DigitizingPaddle.h"
#include "FairLogger.h"
#include "Rtypes.h"
#include <cmath>
#include <functional>
#include <map>
#include <memory>

namespace Digitizing
{
    class DigitizingEngineInterface
    {
      public:
        DigitizingEngineInterface() = default;
        // rule of 5
        virtual ~DigitizingEngineInterface() = default;
        DigitizingEngineInterface(const DigitizingEngineInterface& other) = delete;
        auto operator=(const DigitizingEngineInterface& other) -> DigitizingEngineInterface& = delete;
        DigitizingEngineInterface(DigitizingEngineInterface&& other) = default;
        auto operator=(DigitizingEngineInterface&& other) -> DigitizingEngineInterface& = delete;

        virtual void DepositLight(int paddle_id, double time, double light, double dist) = 0;
        [[nodiscard]] virtual auto GetTriggerTime() const -> double = 0;
        virtual auto ExtractPaddles() -> std::map<int, std::unique_ptr<Paddle>> = 0;
    };

    // factory classes for paddle and channel:
    template <typename ChannelClass,
              typename = typename std::enable_if<std::is_base_of<Channel, ChannelClass>::value>::type>
    class UseChannel
    {
      public:
        template <typename... Args>
        explicit UseChannel(Args&&... args)
            : BuildChannel([&](ChannelSide side)
                           { return std::make_unique<ChannelClass>(side, std::forward<Args>(args)...); })
        {
        }
        std::function<std::unique_ptr<ChannelClass>(ChannelSide)> BuildChannel;
    };

    template <typename PaddleClass,
              typename = typename std::enable_if<std::is_base_of<Paddle, PaddleClass>::value>::type>
    class UsePaddle
    {
      public:
        template <typename... Args>
        explicit UsePaddle(Args&&... args)
            : BuildPaddle([&](int paddleID)
                          { return std::make_unique<PaddleClass>(paddleID, std::forward<Args>(args)...); })
        {
        }
        std::function<std::unique_ptr<PaddleClass>(int)> BuildPaddle;
    };

    template <typename PaddleClass, typename ChannelClass>
    class DigitizingEngine : public DigitizingEngineInterface
    {
      private:
        UsePaddle<PaddleClass> paddleClass;
        UseChannel<ChannelClass> channelClass;
        std::map<int, std::unique_ptr<Paddle>> paddles;

      public:
        DigitizingEngine(const UsePaddle<PaddleClass>& p_paddleClass, const UseChannel<ChannelClass>& p_channelClass)
            : paddleClass{ p_paddleClass }
            , channelClass{ p_channelClass }
            , DigitizingEngineInterface()
        {
        }

        void DepositLight(int paddle_id, double time, double light, double dist) override
        {
            if (paddles.find(paddle_id) == paddles.end())
            {
                auto newPaddle = paddleClass.BuildPaddle(paddle_id);
                newPaddle->SetChannel(channelClass.BuildChannel(Digitizing::ChannelSide::left));
                newPaddle->SetChannel(channelClass.BuildChannel(Digitizing::ChannelSide::right));
                paddles[paddle_id] = std::move(newPaddle);
            }
            paddles.at(paddle_id)->DepositLight({ time, light, dist });
        }

        [[nodiscard]] auto GetTriggerTime() const -> double override
        {
            auto min_element = std::min_element(paddles.begin(),
                                                paddles.end(),
                                                [](const auto& left, const auto& right)
                                                { return left.second->GetTrigTime() < right.second->GetTrigTime(); });
            return (min_element == paddles.end()) ? NAN : min_element->second->GetTrigTime();
        }

        [[nodiscard]] auto ExtractPaddles() -> std::map<int, std::unique_ptr<Paddle>> override
        {
            return std::move(paddles);
        }
    };

    // helper to create owning digitizingEngine:
    template <typename... Args>
    [[nodiscard]] auto CreateEngine(Args&&... args)
        -> std::unique_ptr<decltype(DigitizingEngine{ std::forward<Args>(args)... })>
    {
        return std::make_unique<decltype(DigitizingEngine{ std::forward<Args>(args)... })>(std::forward<Args>(args)...);
    }

} // namespace Digitizing

#endif // NEULAND_DIGITIZING_ENGINE_H
