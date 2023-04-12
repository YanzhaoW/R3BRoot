#pragma once
#include "DigitizingChannel.h"
#include "DigitizingPaddle.h"

namespace Digitizing::Neuland
{
    class MockChannel : public Digitizing::Channel
    {
      public:
        explicit MockChannel(ChannelSide side)
            : Digitizing::Channel{ side } {};
        void AddHit(Hit newHit) override
        {
            InvalidateSignals();
            InvalidateTrigTime();

            auto signal = Signal{};
            signal.qdc = newHit.light;
            signal.qdcUnSat = newHit.light;
            signal.tdc = newHit.time;
            signal.side = GetSide();
            m_Signals.emplace_back(signal);
        }

        void AttachToPaddle(Digitizing::Paddle* paddle) override { SetPaddle(paddle); }

      private:
        Signals m_Signals{};
        auto ConstructSignals() -> Signals override { return m_Signals; }
    };
} // namespace Digitizing::Neuland
