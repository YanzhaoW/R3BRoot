/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019-2024 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BNeulandDigitizer.h"
#include <R3BShared.h>
#include <TFile.h>
#include <iostream>
#include <range/v3/view.hpp>
#include <utility>

R3BNeulandDigitizer::R3BNeulandDigitizer()
    : R3BNeulandDigitizer(Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(), UseChannel<TacquilaChannel>()))
{
}

R3BNeulandDigitizer::R3BNeulandDigitizer(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine)
    : FairTask("R3BNeulandDigitizer")
    , fDigitizingEngine(std::move(engine))
{
}

void R3BNeulandDigitizer::SetEngine(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine)
{
    fDigitizingEngine = std::move(engine);
}

void R3BNeulandDigitizer::SetParContainers()
{
    FairRunAna* run = FairRunAna::Instance();
    if (run == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No analysis run";
    }

    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    if (rtdb == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No runtime database";
    }

    fNeulandGeoPar = dynamic_cast<R3BNeulandGeoPar*>(rtdb->getContainer("R3BNeulandGeoPar"));
    if (fNeulandGeoPar == nullptr)
    {
        LOG(fatal) << "R3BNeulandDigitizer::SetParContainers: No R3BNeulandGeoPar";
    }

    fDigitizingEngine->Init();
}

InitStatus R3BNeulandDigitizer::Init()
{
    fPoints.init();
    fHits.init();
    fCalHits.init();

    // Initialize control histograms
    auto const PaddleMulSize = 3000;
    hMultOne = R3B::root_owned<TH1I>(
        "MultiplicityOne", "Paddle multiplicity: only one PMT per paddle", PaddleMulSize, 0, PaddleMulSize);
    hMultTwo = R3B::root_owned<TH1I>(
        "MultiplicityTwo", "Paddle multiplicity: both PMTs of a paddle", PaddleMulSize, 0, PaddleMulSize);
    auto const timeBinSize = 200;
    hRLTimeToTrig = R3B::root_owned<TH1F>("hRLTimeToTrig", "R/Ltime-triggerTime", timeBinSize, -100., 100.);

    return kSUCCESS;
}

void R3BNeulandDigitizer::Exec(Option_t* /*option*/)
{
    const auto GeVToMeVFac = 1000.;

    std::map<UInt_t, Double_t> paddleEnergyDeposit;
    // Look at each Land Point, if it deposited energy in the scintillator, store it with reference to the bar
    for (const auto& point : fPoints.get())
    {
        LOG(debug) << " input: eloss  " << point.GetEnergyLoss() << std::endl;
        if (point.GetEnergyLoss() > 0.)
        {
            const Int_t paddleID = point.GetPaddle();

            // Convert position of point to paddle-coordinates, including any rotation or translation
            const TVector3 position = point.GetPosition();
            const TVector3 converted_position = fNeulandGeoPar->ConvertToLocalCoordinates(position, paddleID);
            LOG(debug2) << "NeulandDigitizer: Point in paddle " << paddleID
                        << " with global position XYZ: " << position.X() << " " << position.Y() << " " << position.Z();
            LOG(debug2) << "NeulandDigitizer: Converted to local position XYZ: " << converted_position.X() << " "
                        << converted_position.Y() << " " << converted_position.Z();

            // Within the paddle frame, the relevant distance of the light from the pmt is always given by the
            // X-Coordinate
            const Double_t dist = converted_position.X();
            fDigitizingEngine->DepositLight(paddleID, point.GetTime(), point.GetLightYield() * GeVToMeVFac, dist);
            paddleEnergyDeposit[paddleID] += point.GetEnergyLoss() * GeVToMeVFac;
        } // eloss
    }     // points

    const Double_t triggerTime = fDigitizingEngine->GetTriggerTime();
    const auto paddles = fDigitizingEngine->ExtractPaddles();

    // Fill control histograms
    hMultOne->Fill(static_cast<int>(std::count_if(
        paddles.begin(), paddles.end(), [](const auto& keyValue) { return keyValue.second->HasHalfFired(); })));

    hMultTwo->Fill(static_cast<int>(std::count_if(
        paddles.begin(), paddles.end(), [](const auto& keyValue) { return keyValue.second->HasFired(); })));

    hRLTimeToTrig->Fill(triggerTime);

    // Create Hits
    fHits.clear();
    auto& hits = fHits.get();
    for (const auto& [paddleID, paddle] : paddles)
    {
        if (!paddle->HasFired())
        {
            continue;
        }

        auto signals = paddle->GetSignals();

        for (const auto& signal : signals)
        {
            const TVector3 hitPositionLocal = TVector3(signal.position, 0., 0.);
            const TVector3 hitPositionGlobal = fNeulandGeoPar->ConvertToGlobalCoordinates(hitPositionLocal, paddleID);
            const TVector3 hitPixel = fNeulandGeoPar->ConvertGlobalToPixel(hitPositionGlobal);

            R3BNeulandHit hit(paddleID,
                              signal.leftChannel.tdc,
                              signal.rightChannel.tdc,
                              signal.time,
                              signal.leftChannel.qdcUnSat,
                              signal.rightChannel.qdcUnSat,
                              signal.energy,
                              hitPositionGlobal,
                              hitPixel);

            if (fHitFilters.IsValid(hit))
            {
                hits.push_back(std::move(hit));
                LOG(debug) << "Adding neuland hit with id = " << paddleID << ", time = " << signal.time
                           << ", energy = " << signal.energy;
                LOG(debug) << "Adding neuland hit with id = " << paddleID
                           << ", tot_l = " << signal.leftChannel.qdcUnSat * 15 + 14
                           << ", tot_r = " << signal.rightChannel.qdcUnSat * 15 + 14;
            }
        } // loop over all hits for each paddle
    }     // loop over paddles

    if (is_cal_output_)
    {
        fill_cal_data(paddles);
    }
    LOG(debug) << "R3BNeulandDigitizer: produced " << hits.size() << " hits";
}

void R3BNeulandDigitizer::fill_cal_data(const std::map<int, std::unique_ptr<R3B::Digitizing::Paddle>>& paddles)
{
    fCalHits.clear();
    auto& cal_hits = fCalHits.get();
    for (const auto& [paddleID, paddle] : paddles)
    {
        if (!paddle->HasFired())
        {
            continue;
        }

        auto& left_channel = paddle->GetLeftChannelRef();
        auto& right_channel = paddle->GetRightChannelRef();

        auto left_channel_signals = left_channel.GetCalSignals();
        auto right_channel_signals = right_channel.GetCalSignals();

        // LOG(error)<< " Sum pmt_peak_: "<<
        // std::accumulate(right_channel.pmt_peaks_.begin(),right_channel.pmt_peaks_.end(),0)<<std::endl;
        for (const auto& [left, right] : ranges::zip_view(left_channel_signals, right_channel_signals))
        {

            auto cal_data = R3B::Neuland::SimCalData{ paddleID, left.tot, right.tot, left.tle, right.tle };

            if (fCalHitFilters.IsValid(cal_data))
            {
                cal_hits.push_back(std::move(cal_data));
                LOG(debug) << "Adding cal with id = " << paddleID << " left tot " << left.tot << " right tot "
                           << right.tot << std::endl;
            }
        } // loop over all hits for each paddle
    }     // loop over paddles

    LOG(debug) << "R3BNeulandDigitizerCalData: produced " << cal_hits.size() << " hits";
}

void R3BNeulandDigitizer::Finish()
{
    TDirectory* tmp = gDirectory;
    FairRootManager::Instance()->GetOutFile()->cd();

    gDirectory->mkdir("R3BNeulandDigitizer");
    gDirectory->cd("R3BNeulandDigitizer");

    hMultOne->Write();
    hMultTwo->Write();

    gDirectory = tmp;
}

ClassImp(R3BNeulandDigitizer); // NOLINT
