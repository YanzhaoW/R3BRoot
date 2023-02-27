#pragma once
/**
 * Simulation of Mock Bar/Paddle
 * @author Yanzhao Wang
 * @since March 13, 2023
 *
 *
 */

#include "DigitizingPaddle.h"

namespace Digitizing::Neuland
{
    class MockPaddle : public Digitizing::Paddle
    {
      public:
        explicit MockPaddle(uint16_t paddleID)
            : Digitizing::Paddle{ paddleID }
        {
        }

      private:
        [[nodiscard]] auto ComputeTime(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
            -> double override
        {
            return (firstSignal.tdc + secondSignal.tdc) / 2;
        }
        [[nodiscard]] auto ComputeEnergy(const Channel::Signal& firstSignal, const Channel::Signal& secondSignal) const
            -> double override
        {
            return (firstSignal.qdcUnSat + secondSignal.qdcUnSat) / 2;
        }
        [[nodiscard]] auto ComputePosition(const Channel::Signal& leftSignal, const Channel::Signal& rightSignal) const
            -> double override
        {
            if (leftSignal.side == rightSignal.side)
            {
                LOG(fatal)
                    << "DigitizingPaddleNeuland.cxx::ComputePosition(): cannot comput position with signals from "
                       "same side!";
                return 0.F;
            }
            return (leftSignal.side == ChannelSide::left) ? (rightSignal.tdc - leftSignal.tdc) / 2 * gCMedium
                                                          : (leftSignal.tdc - rightSignal.tdc) / 2 * gCMedium;
        }
        auto ComputeChannelHits(const Hit& hit) const -> Pair<Channel::Hit> override
        {
            auto rightChannelHit = GenerateMockChannelHit(hit.time, hit.LightDep, hit.DistToPaddleCenter);
            auto leftChannelHit = GenerateMockChannelHit(hit.time, hit.LightDep, -1 * hit.DistToPaddleCenter);
            return { leftChannelHit, rightChannelHit };
        }

      public:
        static constexpr double gHalfLength = 135.; // [cm]
        static constexpr double gCMedium = 30.;     // speed of light in material in [cm/ns]
        static auto GenerateMockChannelHit(Double_t mcTime, Double_t mcLight, Double_t dist) -> Channel::Hit
        {
            auto time = mcTime + (MockPaddle::gHalfLength + dist) / MockPaddle::gCMedium;
            auto light = mcLight;
            return { time, light };
        }
    };
} // namespace Digitizing::Neuland
