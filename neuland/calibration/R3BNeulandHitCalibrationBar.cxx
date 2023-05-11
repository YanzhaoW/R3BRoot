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

#include "R3BNeulandHitCalibrationBar.h"
#include "R3BNeulandHitModulePar.h"
#include "R3BNeulandHitPar.h"

#include "FairLogger.h"

#include "TCanvas.h"
#include "TDirectory.h"
#include "TGaxis.h"
#include "TGraph.h"
#include "TPad.h"

#include <limits>
#include <numeric>
#include <string>

using DPair = std::array<Double_t, 2>;

// FIXME namespace Neuland::Calibration { with c++17
namespace R3B::Neuland::Calibration
{
    bool GetJumps(double* data,
                  unsigned int size,
                  double_t threshold,
                  unsigned int averageSize,
                  std::vector<unsigned int>* output)
    {
        if (size < 2 * averageSize)
            return false;

        const auto scale = 1. / averageSize;

        auto lastAverage = std::accumulate(data, data + averageSize, 0.) * scale;
        const auto last = data + size;
        auto currentPos = data + averageSize;

        while (currentPos + averageSize < last)
        {
            const auto currentAverage = std::accumulate(currentPos, currentPos + averageSize, 0.) * scale;
            if (fabs(currentAverage - lastAverage) > threshold)
            {
                if (output == nullptr)
                    return true;
                output->push_back(currentPos - data);
            }
            lastAverage = currentAverage;
            currentPos += averageSize;
        }

        return false;
    }

    template <Int_t iterations = 8>
    const double FastExp(const double val)
    {
        auto exp = 1. + val / (1 << iterations);
        for (auto i = 0; i < iterations; ++i)
        {
            exp *= exp;
        }
        return exp;
    }

    const double GetLightAtPMT(const double energyLoss,
                               const double position,
                               const double invLightAttenuationLength,
                               const int side)
    {
        return energyLoss * FastExp(-invLightAttenuationLength * (TotalBarLength * 0.5 + (1 - 2 * side) * position));
    }

    constexpr auto RightSide = 0;
    constexpr auto LeftSide = 1;

    constexpr auto RightSideHitBit = RightSide;
    constexpr auto LeftSideHitBit = LeftSide;
    constexpr auto PosCalibrationBit = 2;
    constexpr auto TSyncCalibrationBit = 3;
    constexpr auto EnergyCalibrationBit = 4;
    constexpr auto ThresholdCalibrationBit = 5;
    constexpr auto PedestalCalibrationBit = 6;
    constexpr auto TimeJumpedBit = 20;
    constexpr auto LogCompleteBit = 30;
    constexpr auto StrangeBit = 31;

    constexpr auto PositionCalibrationSize = 1024;
    constexpr auto EnergyCalibrationSize = 4 * 1024;
    constexpr auto ThresholdCalibrationSize = 1024;
    constexpr auto CalibrationLogSize = 16 * 1024;
    constexpr auto LogInitialSize = 128;

    constexpr auto MinEnergyDeposit = 1.;     // MeV
    constexpr auto MaxTSyncError = 0.05;      // ns
    constexpr auto TimeJumpThreshold = 0.075; // ns
    constexpr auto MaxFastTDiffError = 0.05;

    constexpr auto MaxNumberOfFails = 10U;

    const void SetStatus(Int_t& var, Int_t statusBit) { var |= (1 << statusBit); }
    const void ClearStatus(Int_t& var, Int_t statusBit) { var &= ~(1 << statusBit); }
    const bool IsStatus(Int_t var, Int_t statusBit) { return (var & (1 << statusBit)); }

    const char* HitCalibrationBar::CalibrationStatusDescription[] = { "No Data recorded",
                                                                      "Calibration Failed",
                                                                      "Calibration was Successful",
                                                                      "Energy Calibration Failed",
                                                                      "Time Synchronisation Failed",
                                                                      "Time Synchronisation Error large",
                                                                      "Parameters look strange",
                                                                      "Time Jump" };
    const char* HitCalibrationBar::CalibrationStatusAbbreviation[] = { "XX", "FA", "  ", "EC", "TS", "SE", "??", "TJ" };

    HitCalibrationBar::HitCalibrationBar(const Int_t id)
        : ID_(id)
        , validity_(0)
        , failCounter_(0)
        , lastEventNumber_(0)
        , TimeDifference(NaN)
        , EffectiveSpeed(NaN)
        , Gain({ NaN, NaN })
        , InvLightAttenuationLength(NaN)
        , Pedestal({ 0, 0 })
        , Saturation({ 0., 0. })
        , TimeSync({ NaN, NaN })
        , Threshold({ NaN, NaN })
    {

        Reset();
        lastHits_.reserve(CalibrationLogSize);

        log_.TimeDifference.Expand(LogInitialSize);
        log_.TimeDifference.SetTitle("Time Difference Parameter;Event Number;Parameter Value / ns");

        log_.EffectiveSpeed.Expand(LogInitialSize);
        log_.EffectiveSpeed.SetTitle("Effective Speed Parameter;Event Number;Parameter Value / cm/ns");

        log_.LightAttenuationLength.Expand(LogInitialSize);
        log_.LightAttenuationLength.SetTitle("Attenuation Length Parameter;Event Number;Parameter Value / cm");

        for (auto side = 0; side < 2; ++side)
        {
            log_.Gain[side].Expand(LogInitialSize);
            log_.Gain[side].SetTitle("Gain Parameter;Event Number;Parameter Value / ns/MeVle");

            log_.TotalHits[side] = TH1F("", "Total Hits; Light at PMT / MeVle; #Hits", 64, 0., 32.);
            log_.TotalHits[side].SetDirectory(nullptr);
            log_.TotalHits[side].SetStats(false);

            log_.MissedHits[side] = TH1F("", "Missed Hits; Light at PMT / MeVle; #Hits", 64, 0., 32.);
            log_.MissedHits[side].SetDirectory(nullptr);
            log_.MissedHits[side].SetStats(false);
            log_.MissedHits[side].SetLineColor(kGreen);

            log_.Energy[side] = TH1F("", "Energy Calibration; Gain / ns/MeVle; Counts", 100, 0., 30.);
            log_.Energy[side].SetDirectory(nullptr);
            log_.Energy[side].SetStats(false);

            log_.Saturation[side] =
                TH2F("", "Saturation; Light at PMT / MeVle; QDC / ns", 128, 0., 128., 256, 0., 512.);
            log_.Saturation[side].SetDirectory(nullptr);
            log_.Saturation[side].SetStats(false);

            log_.Pedestal[side] = TH1F("", "Pedestal; QDC / ns", 512, 0., 512.);
            log_.Pedestal[side].SetDirectory(nullptr);
            log_.Pedestal[side].SetStats(false);
        }

        FitGraph.Expand(PositionCalibrationSize);
    }

    HitCalibrationBar::~HitCalibrationBar() {}

    void HitCalibrationBar::Update(const R3BNeulandHitModulePar* par)
    {
        if (!std::isnan(par->GetTimeOffset(1)))
        {
            TimeDifference = par->GetTDiff();
            EffectiveSpeed = par->GetEffectiveSpeed();
            SetStatus(validity_, PosCalibrationBit);
        }

        if (!std::isnan(par->GetEnergyGain(1)))
        {
            Gain[0] = par->GetEnergyGain(1);
            Gain[1] = par->GetEnergyGain(2);

            InvLightAttenuationLength = 1. / par->GetLightAttenuationLength();

            SetStatus(validity_, EnergyCalibrationBit);
        }

        if (!std::isnan(par->GetPMTThreshold(1)))
        {
            Threshold[0] = par->GetPMTThreshold(1);
            Threshold[1] = par->GetPMTThreshold(2);
            SetStatus(validity_, ThresholdCalibrationBit);
        }

        if (!std::isnan(par->GetPMTSaturation(1)))
        {
            Saturation[0] = par->GetPMTSaturation(1);
            Saturation[1] = par->GetPMTSaturation(2);
        }

        if (!std::isnan(par->GetPedestal(1)))
        {
            Pedestal[0] = par->GetPedestal(1);
            Pedestal[1] = par->GetPedestal(2);
            SetStatus(validity_, PedestalCalibrationBit);
        }

        LOG(debug) << "Loaded Parameters for Bar " << ID_;
        return;
    }

    void HitCalibrationBar::Set(const Int_t side, const Double_t time, const Int_t qdc)
    {
        SetStatus(validity_, side);

        currentHit_.Time[side] = time;
        currentHit_.QDC[side] = std::max(qdc - Pedestal[side], 1);
        log_.Pedestal[side].Fill(qdc);

        if (IsStatus(validity_, LeftSideHitBit) && IsStatus(validity_, RightSideHitBit))
        {
            if (fabs(currentHit_.Time[1] - currentHit_.Time[0]) > 0.5 * MaxCalTime)
            {
                if (currentHit_.Time[1] < currentHit_.Time[0])
                    currentHit_.Time[1] += MaxCalTime;
                else
                    currentHit_.Time[0] += MaxCalTime;
            }

            if (IsStatus(validity_, PosCalibrationBit))
            {
                currentHit_.EntryPosition =
                    (currentHit_.Time[1] - currentHit_.Time[0] - TimeDifference) * EffectiveSpeed;

                if (fabs(currentHit_.EntryPosition) > TotalBarLength)
                {
                    ++failCounter_;
                    if (failCounter_ > 10)
                    {
                        LOG(debug) << "Bar " << ID_ << ":"
                                   << " Positions were too often too strange. Might be a Time-Jump in one PMT. "
                                      "Removing Position-Calibration.";
                        failCounter_ = 0;
                        ClearStatus(validity_, PosCalibrationBit);
                    }
                }
                else
                {
                    failCounter_ = 0;
                }
            }
        }
    }
    void HitCalibrationBar::SetGlobalTSync(const Double_t value, const Double_t error)
    {
        TimeSync = { value, error };
        if (std::isnan(value))
            ClearStatus(validity_, TSyncCalibrationBit);
        else
            SetStatus(validity_, TSyncCalibrationBit);
    }

    HitCalibrationBar::CalibrationStatus HitCalibrationBar::GetCalibrationStatus() const
    {
        if (lastHits_.size() == 0 && !IsStatus(validity_, LogCompleteBit))
            return CalibrationStatus::noData;
        if (!IsStatus(validity_, PosCalibrationBit))
            return CalibrationStatus::completeFail;
        if (!IsStatus(validity_, EnergyCalibrationBit))
            return CalibrationStatus::energyGainFail;
        if (!IsStatus(validity_, TSyncCalibrationBit))
            return CalibrationStatus::timeSyncFail;
        if (IsStatus(validity_, TimeJumpedBit))
            return CalibrationStatus::timeJump;
        if (TimeSync[1] > MaxTSyncError)
            return CalibrationStatus::timeSyncError;
        if (IsStatus(validity_, StrangeBit))
            return CalibrationStatus::strangeParameters;
        return CalibrationStatus::success;
    }

    Bool_t HitCalibrationBar::Add(const Double_t timeOffset,
                                  const Double_t entryPosition,
                                  const Double_t exitPosition,
                                  const Double_t energy,
                                  const UInt_t eventNumber)
    {
        // If we have already a gain calibration, we can log the hits.
        // This way we might get some information about strange behaviour.
        if (IsStatus(validity_, EnergyCalibrationBit))
        {
            const auto centerPosition = (entryPosition + exitPosition) * 0.5;
            const DPair lightAtPMT = { GetLightAtPMT(energy, centerPosition, InvLightAttenuationLength, RightSide),
                                       GetLightAtPMT(energy, centerPosition, InvLightAttenuationLength, LeftSide) };

            if (!IsStatus(validity_, RightSideHitBit))
            {
                log_.MissedHits[RightSide].Fill(lightAtPMT[RightSide]);
            }
            if (!IsStatus(validity_, LeftSideHitBit))
            {
                log_.MissedHits[LeftSide].Fill(lightAtPMT[LeftSide]);
            }

            for (auto side = 0; side < 2; ++side)
                log_.TotalHits[side].Fill(lightAtPMT[side]);

            if (IsStatus(validity_, LeftSideHitBit) && IsStatus(validity_, RightSideHitBit))
            {
                for (auto side = 0; side < 2; ++side)
                {
                    log_.Saturation[side].Fill(lightAtPMT[side], currentHit_.QDC[side]);
                    const auto gain = currentHit_.QDC[side] /
                                      (lightAtPMT[side] * (1. - currentHit_.QDC[side] * SaturationCoefficient));
                    log_.Energy[side].Fill(gain);
                }
            }
        }

        // We require both sides and reasonable energy deposit.
        if (!IsStatus(validity_, LeftSideHitBit) || !IsStatus(validity_, RightSideHitBit) || energy < MinEnergyDeposit)
            return false;

        // Dont take the bar, if the cosmic was somewhere close to the lightguide:
        // Strange things happen there ;).
        const auto maxPos = BarLength * 0.5 - 1.;
        if (fabs(entryPosition) > maxPos || fabs(exitPosition) > maxPos)
            return false;

        lastEventNumber_ = eventNumber;

        for (auto side = 0; side < 2; ++side)
            currentHit_.Time[side] -= timeOffset * 0.5;

        currentHit_.EntryPosition = entryPosition;
        currentHit_.ExitPosition = exitPosition;
        currentHit_.Energy = energy;

        lastHits_.push_back(currentHit_);

        if (lastHits_.size() % PositionCalibrationSize == 0)
            positionCalibration(lastHits_.size() - PositionCalibrationSize, PositionCalibrationSize);

        if (lastHits_.size() % EnergyCalibrationSize == 0)
            energyCalibration(lastHits_.size() - EnergyCalibrationSize, EnergyCalibrationSize);

        // If we reached our limit, clear the vector.
        // The content itself is still there. We will set a (hacky) bit,
        // so we know when the vector actually contains more data than its size.
        if (lastHits_.size() == CalibrationLogSize)
        {
            SetStatus(validity_, LogCompleteBit);
            lastHits_.clear();
        }

        // We can say only that this was a somewhat good event, if we have a position calibration
        // and the position of the track matches with the one from the calibration
        if (IsStatus(validity_, PosCalibrationBit) && fabs(0.5 * (entryPosition + exitPosition) - GetPosition()) > 25.)
            return true;

        return true;
    }

    void HitCalibrationBar::Reset()
    {
        ClearStatus(validity_, LeftSideHitBit);
        ClearStatus(validity_, RightSideHitBit);
        currentHit_.EntryPosition = NaN;
    }

    void HitCalibrationBar::Calibrate(TDirectory* histogramDir)
    {
        const auto nHits = (IsStatus(validity_, LogCompleteBit) ? CalibrationLogSize : lastHits_.size());

        // Check if we have any Hits at all.
        // If not, this bar is either not connected or has some problems.
        // In this case do not bother creating plots and just return.
        if (nHits == 0)
            return;

        // Position Calibration
        if (log_.TimeDifference.GetN() > 0)
        {
            TimeDifference = getMean(log_.TimeDifference);

            EffectiveSpeed = getMean(log_.EffectiveSpeed);

            // check for timejumps
            if (GetJumps(log_.TimeDifference.GetY(), log_.TimeDifference.GetN(), TimeJumpThreshold, 3, nullptr))
                SetStatus(validity_, TimeJumpedBit);

            SetStatus(validity_, PosCalibrationBit);
        }
        else if (nHits > 0.5 * PositionCalibrationSize)
        {
            // There were not enough hits for the usual calibration.
            // If we have at least half the chunk size, we can try to calibrate anyway
            positionCalibration(0, nHits);
        }
        else
        {
            // We cannot have a reliable calibration.
            // Give up at this point.
            ClearStatus(validity_, PosCalibrationBit);
        }

        // Gain Calibration
        if (log_.Gain[0].GetN() > 0 && log_.Gain[1].GetN() > 0)
        {
            for (auto side = 0; side < 2; ++side)
            {
                Gain[side] = getMean(log_.Gain[side], Gain[side]);
                Saturation[side] = SaturationCoefficient * Gain[side];
            }

            InvLightAttenuationLength = 1. / getMean(log_.LightAttenuationLength, 1. / InvLightAttenuationLength);

            SetStatus(validity_, EnergyCalibrationBit);
        }
        else if (nHits > 0.5 * EnergyCalibrationSize)
        {
            // There were not enough hits for the usual calibration.
            // If we have at least half the chunk size, we can try to calibrate anyway
            energyCalibration(0, nHits);
        }
        else
        {
            // We cannot have a reliable calibration.
            // Give up at this point.
            ClearStatus(validity_, EnergyCalibrationBit);
        }

        // Threshold Calibration
        thresholdCalibration();

        createHistograms(histogramDir);
    }

    R3BNeulandHitModulePar HitCalibrationBar::GetParameters()
    {
        R3BNeulandHitModulePar parameter;
        parameter.SetModuleId(ID_);

        parameter.SetTDiff(TimeDifference);
        parameter.SetTSync(TimeSync[0]);
        parameter.SetEffectiveSpeed(EffectiveSpeed);
        parameter.SetLightAttenuationLength(1. / InvLightAttenuationLength);

        for (auto side = 0; side < 2; ++side)
        {
            parameter.SetEnergyGain(Gain[side], side + 1);
            parameter.SetPMTThreshold(Threshold[side], side + 1);
            parameter.SetPMTSaturation(Saturation[side], side + 1);
            parameter.SetPedestal(Pedestal[side], side + 1);
        }

        LOG(debug) << ID_ << " Bar Parameters: ";
        LOG(debug) << "   Time Offset       : " << parameter.GetTimeOffset(1) << " " << parameter.GetTimeOffset(2);
        LOG(debug) << "   Effective Speed   : " << parameter.GetEffectiveSpeed();
        LOG(debug) << "   Energy Gain       : " << parameter.GetEnergyGain(1) << " " << parameter.GetEnergyGain(2);
        LOG(debug) << "   Attenuationlength : " << parameter.GetLightAttenuationLength();
        LOG(debug) << "   Threshold         : " << parameter.GetPMTThreshold(1) << " " << parameter.GetEnergyGain(2);
        LOG(debug) << "   Pedestal          : " << parameter.GetPedestal(1) << " " << parameter.GetPedestal(2);
        LOG(debug) << "   Saturation        : " << parameter.GetPMTSaturation(1) << " "
                   << parameter.GetPMTSaturation(2);

        return parameter;
    }

    Bool_t HitCalibrationBar::IsValid() const
    {
        return IsStatus(validity_, LeftSideHitBit) && IsStatus(validity_, RightSideHitBit);
    }

    void HitCalibrationBar::positionCalibration(int firstHit, int nHits)
    {
        FitGraph.Set(0);
        for (auto p = 0; p < nHits; ++p)
        {
            const auto& hit = lastHits_[firstHit + p];

            const auto meanPosition = 0.5 * (hit.EntryPosition + hit.ExitPosition);
            const auto tdiff = hit.Time[1] - hit.Time[0];
            FitGraph.SetPoint(p, meanPosition, tdiff);
        }

        TF1 linearFit("", "pol1", -TotalBarLength * 0.5, TotalBarLength * 0.5);
        FitGraph.Fit(&linearFit, "NQ");
        if (linearFit.GetParError(0) > MaxFastTDiffError)
        {
            cleanupFit(FitGraph, linearFit, 2.5);
        }

        // Use this calibration
        TimeDifference = linearFit.GetParameter(0);
        EffectiveSpeed = 1. / linearFit.GetParameter(1);

        // Write parameters to the Log.
        const auto nPoints = log_.TimeDifference.GetN();
        log_.TimeDifference.SetPoint(nPoints, lastEventNumber_, TimeDifference);
        log_.TimeDifference.SetPointError(nPoints, 0, linearFit.GetParError(0));
        log_.EffectiveSpeed.SetPoint(nPoints, lastEventNumber_, EffectiveSpeed);
        log_.EffectiveSpeed.SetPointError(nPoints, 0, linearFit.GetParError(1) * Sqr(EffectiveSpeed));

        SetStatus(validity_, PosCalibrationBit);
    }

    void HitCalibrationBar::energyCalibration(int firstHit, int nHits)
    {
        if (!IsStatus(validity_, PedestalCalibrationBit))
        {
            // This is the first calibration
            // if we do not have pedestals yet, get them and subtract them from the hits

            pedestalCalibration();
            for (auto h = 0; h < nHits; ++h)
            {
                auto& hit = lastHits_[firstHit + h];
                for (auto side = 0; side < 2; ++side)
                    hit.QDC[side] = std::max(hit.QDC[side] - Pedestal[side], 1);
            }
        }

        FitGraph.Set(0);
        for (auto h = 0; h < nHits; ++h)
        {
            const auto& hit = lastHits_[firstHit + h];
            const auto centerPosition = (hit.EntryPosition + hit.ExitPosition) * 0.5;
            FitGraph.SetPoint(h, centerPosition, log(hit.QDC[1] * 1. / hit.QDC[0]));
        }

        TF1 linearFit("", "pol1", -TotalBarLength * 0.5, TotalBarLength * 0.5);
        // this has usually always outliers, so do not bother doing a normal fit first.
        cleanupFit(FitGraph, linearFit, 1.);

        InvLightAttenuationLength = 0.5 * linearFit.GetParameter(1);

        const auto logLightAttLenPoints = log_.LightAttenuationLength.GetN();
        log_.LightAttenuationLength.SetPoint(logLightAttLenPoints, lastEventNumber_, 1. / InvLightAttenuationLength);
        log_.LightAttenuationLength.SetPointError(
            logLightAttLenPoints, 0, 0.5 * linearFit.GetParError(1) / Sqr(InvLightAttenuationLength));

        for (auto h = 0; h < nHits; ++h)
        {
            const auto& hit = lastHits_[firstHit + h];
            const auto centerPosition = (hit.EntryPosition + hit.ExitPosition) * 0.5;
            for (auto side = 0; side < 2; ++side)
            {
                const auto lightAtPMT = GetLightAtPMT(hit.Energy, centerPosition, InvLightAttenuationLength, side);
                const auto gain = hit.QDC[side] / (lightAtPMT * (1. - hit.QDC[side] * SaturationCoefficient));
                log_.Energy[side].Fill(gain);
            }
        }

        std::array<TH1F, 2> hGainCal = { TH1F("", "", 200, 0., 40.), TH1F("", "", 200, 0., 40.) };

        for (auto h = 0; h < nHits; ++h)
        {
            const auto& hit = lastHits_[firstHit + h];
            const auto centerPosition = (hit.EntryPosition + hit.ExitPosition) * 0.5;
            for (auto side = 0; side < 2; ++side)
            {
                const auto lightAtPMT = GetLightAtPMT(hit.Energy, centerPosition, InvLightAttenuationLength, side);
                const auto distance = 0.5 * TotalBarLength + (1 - 2 * side) * centerPosition;
                const auto gain = hit.QDC[side] / (lightAtPMT * (1. - hit.QDC[side] * SaturationCoefficient));
                hGainCal[side].Fill(gain);
            }
        }

        TF1 gausFit("", "gaus", 0., 30.);
        for (auto side = 0; side < 2; ++side)
        {
            const auto maxPos = hGainCal[side].GetBinCenter(hGainCal[side].GetMaximumBin());
            gausFit.SetParameter(0, hGainCal[side].GetMaximum());
            gausFit.SetParameter(1, maxPos);
            hGainCal[side].Fit(&gausFit, "NQ", "", maxPos - 1.5, maxPos + 1.5);

            Gain[side] = gausFit.GetParameter(1);

            const auto nPoints = log_.Gain[side].GetN();
            log_.Gain[side].SetPoint(nPoints, lastEventNumber_, Gain[side]);
            log_.Gain[side].SetPointError(nPoints, 0, gausFit.GetParError(1));
        }

        SetStatus(validity_, EnergyCalibrationBit);
    }

    void HitCalibrationBar::pedestalCalibration()
    {
        for (auto side = 0; side < 2; ++side)
            for (auto bin = 0; bin < log_.Pedestal[side].GetNbinsX(); ++bin)
            {
                if (log_.Pedestal[side].GetBinContent(bin + 1) > 3.)
                {
                    Pedestal[side] = bin;
                    break;
                }
            }

        SetStatus(validity_, PedestalCalibrationBit);
    }

    void HitCalibrationBar::thresholdCalibration()
    {
        if (log_.TotalHits[0].GetEntries() < ThresholdCalibrationSize ||
            log_.TotalHits[1].GetEntries() < ThresholdCalibrationSize)
            return;

        // We have 50% of the maximum at x=[1]
        // For the bar at the edges it is hard to check if the cosmic was stopped.
        // Therefore we usually have [2] != 0.
        TF1 missFit("missFit", "(1. - [2]) * 0.5 * (1. - TMath::Erf([0] * (x - [1]))) + [2]", 0., 20.);

        for (auto side = 0; side < 2; ++side)
        {
            const auto nPoints = log_.TotalHits[side].GetNbinsX();
            log_.MissRatio[side].Expand(nPoints);
            log_.MissRatio[side].Set(0);
            for (auto bin = 1; bin <= nPoints; ++bin)
            {
                if (log_.MissedHits[side].GetBinContent(bin) > 0)
                {
                    const auto nextPoint = log_.MissRatio[side].GetN();
                    const auto binCenter = log_.MissedHits[side].GetBinCenter(bin);
                    const auto ratio =
                        log_.MissedHits[side].GetBinContent(bin) / log_.TotalHits[side].GetBinContent(bin);
                    log_.MissRatio[side].SetPoint(nextPoint, binCenter, ratio);
                }
            }

            if (log_.MissRatio[side].GetN() > 5)
            {
                missFit.SetParameter(0, 0.25);
                missFit.SetParameter(1, AvgThreshold);
                missFit.SetParLimits(1, 0., 10.);
                missFit.SetParameter(2, 0.);
                missFit.SetParLimits(2, 0., 0.5);
                log_.MissRatio[side].Fit(&missFit, "QNE");
                Threshold[side] = missFit.GetParameter(1);
            }
        }
        SetStatus(validity_, ThresholdCalibrationBit);
    }

    Int_t HitCalibrationBar::cleanupFit(TGraph& graph, TF1& fit, Double_t maxDifference) const
    {
        std::array<Int_t, 256> remove;
        auto totalRemoved = 0;

        graph.Fit(&fit, "QN ROB=0.90");

        auto removePosition = 0;
        auto x = graph.GetX();
        auto y = graph.GetY();
        auto nPoints = graph.GetN();
        for (auto p = 0; p < nPoints; ++p)
        {
            if (fabs(y[p] - fit.Eval(x[p])) < maxDifference)
                continue;

            remove[removePosition] = p;
            if (++removePosition == remove.size())
            {
                removePoints(&remove[0], removePosition, graph);
                totalRemoved += removePosition;
                nPoints -= removePosition;
                p -= removePosition;
                removePosition = 0;

                x = graph.GetX();
                y = graph.GetY();
            }
        }

        removePoints(&remove[0], removePosition, graph);
        totalRemoved += removePosition;

        graph.Fit(&fit, "QN");

        return totalRemoved;
    }

    void HitCalibrationBar::removePoints(Int_t* points, Int_t nPoints, TGraph& graph) const
    {
        if (nPoints == 0)
            return;

        auto offset = 1;
        const auto totalPoints = graph.GetN() - nPoints;
        const auto x = graph.GetX();
        const auto y = graph.GetY();

        for (auto p = points[0]; p < totalPoints; ++p)
        {

            if (offset < nPoints && p + offset == points[offset])
            {
                ++offset;
                --p;
                continue;
            }

            x[p] = x[p + offset];
            y[p] = y[p + offset];
        }

        graph.Set(totalPoints);
    }

    void HitCalibrationBar::createHistograms(TDirectory* histogramDir)
    {
        if (!histogramDir)
            return;

        // First creat all the histograms we do not have yet.
        const auto nHits = (IsStatus(validity_, LogCompleteBit) ? CalibrationLogSize : lastHits_.size());

        TGraph posCal;
        posCal.SetTitle("Position Calibration;Position / cm;Time Difference / ns");
        posCal.Expand(nHits);

        for (auto h = 0; h < nHits; ++h)
        {
            const auto& hit = lastHits_[h];
            const auto tdiff = hit.Time[1] - hit.Time[0];
            posCal.SetPoint(posCal.GetN(), hit.EntryPosition, tdiff);
        }

        std::array<TF1, 2> fSatCal = { TF1("", "[0]*x/(1+[1]*x)", 0., TotalBarLength),
                                       TF1("", "[0]*x/(1+[1]*x)", 0., TotalBarLength) };

        for (auto side = 0; side < 2; ++side)
        {
            fSatCal[side].SetParameter(0, Gain[side]);
            fSatCal[side].FixParameter(1, Saturation[side]);
        }

        // Go into the directory, create a Canvas, fill it with the plots and write it into the directory.
        histogramDir->cd();
        const auto canvname = TString::Format("Bar %d", ID_);
        TCanvas Canvas(canvname, canvname);
        Canvas.SetWindowSize(1920, 1200);
        Canvas.Divide(1, 3);
        auto row = 0;
        TVirtualPad* currentPad;

        currentPad = Canvas.cd(++row);
        currentPad->Divide(3, 1);

        currentPad->cd(1);
        if (posCal.GetN() > 0)
            posCal.Draw("AP");

        currentPad->cd(2);
        if (log_.TimeDifference.GetN() > 0)
            log_.TimeDifference.Draw("A*");

        currentPad->cd(3);
        if (log_.EffectiveSpeed.GetN() > 0)
            log_.EffectiveSpeed.Draw("A*");

        currentPad = Canvas.cd(++row);
        currentPad->Divide(4, 1);

        currentPad->cd(1);
        log_.Energy[0].Draw("");
        log_.Energy[1].SetLineColor(kRed);
        log_.Energy[1].Draw("SAME");

        currentPad->cd(2);
        if (log_.LightAttenuationLength.GetN() > 0)
            log_.LightAttenuationLength.Draw("A*");

        currentPad->cd(3);
        if (log_.Gain[0].GetN() > 0)
            log_.Gain[0].Draw("A*");

        currentPad->cd(4);
        if (log_.Gain[1].GetN() > 0)
            log_.Gain[1].Draw("A*");

        currentPad = Canvas.cd(++row);
        currentPad->Divide(4, 1);

        TPad* mainPads[2];
        TPad* overlayPads[2];
        TGaxis* overlayAxis[2];
        for (auto side = 0; side < 2; ++side)
        {
            // Threshold
            auto pad = currentPad->cd(1 + side);
            mainPads[side] = new TPad("", "", 0, 0, 1, 1);
            mainPads[side]->Draw();
            mainPads[side]->cd();
            log_.TotalHits[side].Draw();
            log_.MissedHits[side].Draw("SAME");
            mainPads[side]->Update();
            pad->cd();

            overlayPads[side] = new TPad("", "", 0, 0, 1, 1);
            overlayPads[side]->SetFillStyle(4000);
            double ymin = 0.;
            double ymax = 1.;
            double dy = (ymax - ymin) / 0.8; // 10 per cent margins top and bottom
            double xmin = mainPads[side]->GetUxmin();
            double xmax = mainPads[side]->GetUxmax();
            double dx = (xmax - xmin) / 0.8; // 10 per cent margins left and right
            overlayPads[side]->Range(xmin - 0.1 * dx, ymin - 0.1 * dy, xmax + 0.1 * dx, ymax + 0.1 * dy);
            overlayPads[side]->Draw();
            overlayPads[side]->cd();

            if (log_.MissRatio[side].GetN() > 0)
            {
                log_.MissRatio[side].SetLineColor(kRed);
                log_.MissRatio[side].Draw("IL");
            }
            overlayPads[side]->Update();
            overlayAxis[side] = new TGaxis(xmax, ymin, xmax, ymax, ymin, ymax, 50510, "+L");
            overlayAxis[side]->SetLabelColor(kRed);
            overlayAxis[side]->Draw();

            overlayPads[side]->Update();

            pad->Modified();

            // Saturation

            currentPad->cd(3 + side);
            log_.Saturation[side].Draw("COLZ");
            fSatCal[side].Draw("SAME");
        }

        Canvas.Write();

        for (auto side = 0; side < 2; ++side)
        {
            delete mainPads[side];
            delete overlayPads[side];
            delete overlayAxis[side];
        }
    }

    Double_t HitCalibrationBar::getMean(const TGraphErrors& graph, Double_t expectedValue)
    {
        const auto nPoints = graph.GetN();
        if (nPoints == 0)
            return NaN;

        if (nPoints == 1)
            return graph.GetY()[0];

        TF1 constantFit("", "pol0", 0, nPoints);

        FitGraph.Set(0);
        for (auto point = 0; point < nPoints; ++point)
        {
            FitGraph.SetPoint(point, point, graph.GetY()[point]);
            FitGraph.SetPointError(point, 0, graph.GetEY()[point]);
        }
        constantFit.SetParameter(0, expectedValue);
        FitGraph.Fit(&constantFit, "QN");
        return constantFit.GetParameter(0);
    }
} // namespace R3B::Neuland::Calibration
