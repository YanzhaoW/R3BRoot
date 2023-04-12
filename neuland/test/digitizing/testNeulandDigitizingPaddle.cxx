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

#include "DigitizingPaddleNeuland.h"
#include "MockModels.h"
#include "gtest/gtest.h"
#include <iostream>
#include <vector>

/**
 * gtest for neuland digitizing modules
 * @author Yanzhao Wang
 * @since April 12, 2023
 */
namespace
{
    using Digitizing::Channel;
    using Digitizing::ChannelSide;
    using Digitizing::Neuland::NeulandPaddle;
    using r3b::testing::Neuland::GMockChannel;
    using ::testing::Return;

    class testNeulandPaddle : public ::testing::Test
    {
      protected:
        testNeulandPaddle() = default;

        void SetUp() override
        {
            auto leftChannel = std::make_unique<GMockChannel>(ChannelSide::left);
            auto rightChannel = std::make_unique<GMockChannel>(ChannelSide::right);
            EXPECT_CALL(*leftChannel, AttachToPaddle).Times(1);
            EXPECT_CALL(*rightChannel, AttachToPaddle).Times(1);
            paddle_->SetChannel(std::move(leftChannel));
            paddle_->SetChannel(std::move(rightChannel));
        }

        void SetExpectCall()
        {
            auto& leftChannel = dynamic_cast<GMockChannel&>(paddle_->GetLeftChannelRef());
            auto& rightChannel = dynamic_cast<GMockChannel&>(paddle_->GetRightChannelRef());
            EXPECT_CALL(leftChannel, ConstructSignals()).Times(1).WillOnce(Return(std::move(leftChannelSignals_)));
            EXPECT_CALL(rightChannel, ConstructSignals()).Times(1).WillOnce(Return(std::move(rightChannelSignals_)));
        }

        template <ChannelSide side>
        void AddChannelSignal(const Channel::Signal& signal)
        {
            if constexpr (side == ChannelSide::right)
            {
                rightChannelSignals_.push_back(signal);
            }
            else
            {
                leftChannelSignals_.push_back(signal);
            }
        }

      private:
        std::unique_ptr<NeulandPaddle> paddle_ = std::make_unique<NeulandPaddle>(0);
        std::vector<Channel::Signal> leftChannelSignals_;
        std::vector<Channel::Signal> rightChannelSignals_;
    };

    TEST_F(testNeulandPaddle, basic) // NOLINT
    {
        SetExpectCall();
    }
} // namespace
