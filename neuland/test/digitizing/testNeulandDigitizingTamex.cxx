/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "DigitizingChannel.h"
#include "DigitizingTamex.h"
#include "gtest/gtest.h"
#include <iostream>

namespace
{
    using TmxChannel = Digitizing::Neuland::Tamex::Channel;
    using TmxPeak = Digitizing::Neuland::Tamex::Peak;
    using Channel = Digitizing::Channel;
    using TmxPar = Digitizing::Neuland::Tamex::Params;

    class testNeulandTamexChannel : public ::testing::Test
    {
      protected:
        testNeulandTamexChannel()
            : fChannel{ std::make_unique<TmxChannel>(Digitizing::ChannelSide::left) }
        {
            SetChannelPar(fChannel->GetPar());
        }

        void SetUp() override {}

        [[nodiscard]] auto GetChannel() const -> TmxChannel* { return fChannel.get(); }

        [[nodiscard]] auto GetPar() const -> TmxPar& { return fChannel->GetPar(); }

        static void SetChannelPar(TmxPar& par)
        {
            par.fPMTThresh = 1.;
            par.fSaturationCoefficient = 0.012;
            par.fExperimentalDataIsCorrectedForSaturation = true;
            par.fEnergyGain = 15.0;
            par.fPedestal = 14.0;
            par.fTimeMax = 1000.;
            par.fTimeMin = 1.;
            par.fQdcMin = 0.67;
        }

        void AddHit(double time, double light) { fChannel->AddHit({ time, light }); }

        [[nodiscard]] auto GetSignals() const -> const Channel::Signals& { return fChannel->GetSignals(); }

        [[nodiscard]] auto GetPeaks() const -> const std::vector<TmxPeak>& { return fChannel->GetFQTPeaks(); }

      private:
        std::unique_ptr<TmxChannel> fChannel = nullptr;
    };

    TEST_F(testNeulandTamexChannel, basic_hit_processing)
    {
        AddHit(20., 20.);
        auto signals = GetSignals();
        auto peaks = GetPeaks();

        // check whether signal got lost
        ASSERT_EQ(peaks.size(), 1) << "no peak is generated!";
        ASSERT_EQ(signals.size(), 1) << "No channel signal is outputted!";

        // check PMT saturation
        ASSERT_LT(peaks[0].GetQDC(), 20.) << "PMT saturation is not implemented!";
        ASSERT_NE(signals[0].tdc, 20.) << "tdc value is not smeared!";
    }

    TEST_F(testNeulandTamexChannel, pmt_threshold_check)
    {
        AddHit(20., 0.5);
        auto signals = GetSignals();
        auto peaks = GetPeaks();
        ASSERT_EQ(signals.size(), 0) << "PMT threshold doesn't filter out low energy signals!";
    }

    TEST_F(testNeulandTamexChannel, pmt_threshold_overlap)
    {
        AddHit(20., 0.5);
        AddHit(20., 0.5);
        AddHit(20., 0.5);
        AddHit(20., 0.5);
        auto signals = GetSignals();
        auto peaks = GetPeaks();
        ASSERT_EQ(signals.size(), 1) << "overlapped signals cannot pass PMT threshold!";
    }

    TEST_F(testNeulandTamexChannel, no_timeRes_check)
    {
        decltype(auto) par = GetPar();
        par.fTimeRes = 0.0;

        AddHit(20., 20.);
        auto signals = GetSignals();
        auto peaks = GetPeaks();
        ASSERT_DOUBLE_EQ(signals[0].tdc, 20.) << "tdc value is not correctly passed on!";
    }

    TEST_F(testNeulandTamexChannel, signal_pileup_check)
    {
        AddHit(20., 20.);
        AddHit(20., 1.5);
        ASSERT_EQ(GetSignals().size(), 1) << "overlapping failed!";

        auto width = GetPeaks().back().GetWidth();
        AddHit(width + 21., 1.5);
        ASSERT_EQ(GetSignals().size(), 2) << "should not be overlapped!";
    }

    TEST_F(testNeulandTamexChannel, signal_multiPileup_check)
    {
        AddHit(20., 5.);
        ASSERT_EQ(GetSignals().size(), 1);
        AddHit(GetPeaks().back().GetTETime() + 5., 5.);
        ASSERT_EQ(GetSignals().size(), 2);
        AddHit(GetPeaks().back().GetTETime() + 5., 5.);
        ASSERT_EQ(GetSignals().size(), 3);
        AddHit(GetPeaks().back().GetTETime() + 5., 5.);
        ASSERT_EQ(GetSignals().size(), 4);
        AddHit(GetPeaks().back().GetTETime() + 5., 5.);
        ASSERT_EQ(GetSignals().size(), 5);
        auto minTime = GetPeaks().front().GetLETime();
        auto maxTime = GetPeaks().back().GetTETime();
        auto qdc_test = TmxPeak::WidthToQdc(maxTime - minTime + 10., GetPar());
        AddHit(minTime - 5., qdc_test);
        ASSERT_NE(GetSignals().size(), 1);
    }
} // namespace
