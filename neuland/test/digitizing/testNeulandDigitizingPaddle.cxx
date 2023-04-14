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
    using Digitizing::PaddleHit;
    using Digitizing::Neuland::NeulandPaddle;
    using r3b::testing::Neuland::GMockChannel;
    using ::testing::Return;
    template <typename Type>
    using Pair = Digitizing::LRPair<Type>;

    class testNeulandPaddle : public ::testing::Test
    {
      protected:
        testNeulandPaddle() = default;

        void SetUp() override
        {
            auto leftChannel = std::make_unique<GMockChannel>(ChannelSide::left);
            auto rightChannel = std::make_unique<GMockChannel>(ChannelSide::right);
            channels_.left = leftChannel.get();
            channels_.right = rightChannel.get();
            EXPECT_CALL(*channels_.left, AttachToPaddle).Times(1);
            EXPECT_CALL(*channels_.right, AttachToPaddle).Times(1);
            paddle_->SetChannel(std::move(leftChannel));
            paddle_->SetChannel(std::move(rightChannel));
        }

        void SetExpectCall()
        {
            EXPECT_CALL(*channels_.left, ConstructSignals()).Times(1).WillOnce(Return(std::move(channelSignals_.left)));
            EXPECT_CALL(*channels_.right, ConstructSignals())
                .Times(1)
                .WillOnce(Return(std::move(channelSignals_.right)));
        }

        template <ChannelSide side>
        void AddChannelSignal(const Channel::Signal& signal)
        {
            if constexpr (side == ChannelSide::right)
            {
                channelSignals_.right.push_back(signal);
            }
            else
            {
                channelSignals_.left.push_back(signal);
            }
        }

        [[nodiscard]] const auto& GetChannels() const { return channels_; }
        [[nodiscard]] auto* GetPaddle() const { return paddle_.get(); }

      private:
        std::unique_ptr<NeulandPaddle> paddle_ = std::make_unique<NeulandPaddle>(0);
        Pair<std::vector<Channel::Signal>> channelSignals_;
        Pair<GMockChannel*> channels_ = { nullptr, nullptr };
    };

    TEST_F(testNeulandPaddle, check_inputN_to_channel) // NOLINT
    {
        auto* paddle = GetPaddle();
        paddle->DepositLight(PaddleHit{ 10., 20., 0. });
        paddle->DepositLight(PaddleHit{ 10., 20., 0. });
        paddle->DepositLight(PaddleHit{ 20., 0., 0. });
        const auto& leftHits = GetChannels().left->hits_;
        const auto& rightHits = GetChannels().right->hits_;
        ASSERT_EQ(leftHits.size(), rightHits.size()) << "Numbers of hits of both PMT channels don't match!";
        ASSERT_EQ(leftHits.size(), 3) << "Number of hits for a PMT channel doesn't match with paddle inputs";
        // SetExpectCall();
    }

    TEST_F(testNeulandPaddle, check_light_attenuation) // NOLINT
    {
        auto* paddle = GetPaddle();
        auto const inputE = 20.;
        auto paddleHit = PaddleHit{ 10., inputE, 0 };
        paddle->DepositLight(paddleHit);
        const auto& leftHits = GetChannels().left->hits_;
        const auto& rightHits = GetChannels().right->hits_;
        ASSERT_LT(leftHits[0].light, inputE) << "No light attenuation for the left channel!";
        ASSERT_LT(rightHits[0].light, inputE) << "No light attenuation for the right channel!";
    }

    TEST_F(testNeulandPaddle, check_positive_direction) // NOLINT
    {
        auto* paddle = GetPaddle();
        auto const inputE = 20.;
        auto const initPos = 0.8 * NeulandPaddle::gHalfLength;
        auto paddleHit = PaddleHit{ 10., inputE, initPos };
        paddle->DepositLight(paddleHit);
        const auto& leftHits = GetChannels().left->hits_;
        const auto& rightHits = GetChannels().right->hits_;
        ASSERT_LT(leftHits[0].light, rightHits[0].light) << "left channel has larger light input with longer distance!";
        ASSERT_GT(leftHits[0].time, rightHits[0].time) << "light hit left channel ealier with longer distance!";
    }
} // namespace
