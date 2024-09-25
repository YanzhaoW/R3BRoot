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

#pragma once
#include "FairTask.h"
#include "Filterable.h"
#include "NeulandSimCalData.h"
#include "R3BDigitizingEngine.h"
#include "R3BDigitizingPaddleNeuland.h"
#include "R3BDigitizingTacQuila.h"
#include "R3BDigitizingTamex.h"
#include "R3BNeulandGeoPar.h"
#include "R3BNeulandHit.h"
#include "R3BNeulandHitPar.h"
#include "R3BNeulandPoint.h"
#include "TCAConnector.h"
#include <R3BIOConnector.h>
#include <TClonesArray.h>
#include <TH1.h>

class TGeoNode;
class TH1F;
class TH2F;

/**
 * NeuLAND digitizing finder task
 * @author Jan Mayer
 *
 * For each event, get the R3BNeulandPoints and apply the detector response to create R3BNeulandHits.
 *   Input:  Branch NeulandPoints = TClonesArray("R3BNeulandPoint")
 *           Stored Neuland Geometry Parameter NeulandGeoPar
 *   Output: Branch NeulandHits  = TClonesArray("R3BNeulandDigi")
 *   Additional output: Some control histograms
 *
 */

namespace Digitizing = R3B::Digitizing;

class R3BNeulandDigitizer : public FairTask
{
  public:
    enum class Options
    {
        neulandTamex,
        neulandTacquila
    };
    using NeulandPaddle = Digitizing::Neuland::NeulandPaddle;
    using TacquilaChannel = Digitizing::Neuland::TacQuila::Channel;
    using TamexChannel = Digitizing::Neuland::Tamex::Channel;
    template <typename Type>
    using UseChannel = Digitizing::UseChannel<Type>;
    template <typename Type>
    using UsePaddle = Digitizing::UsePaddle<Type>;

    explicit R3BNeulandDigitizer();
    explicit R3BNeulandDigitizer(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine);

    ~R3BNeulandDigitizer() override = default;

    // No copy and no move is allowed (Rule of three/five)
    R3BNeulandDigitizer(const R3BNeulandDigitizer&) = delete;            // copy constructor
    R3BNeulandDigitizer(R3BNeulandDigitizer&&) = delete;                 // move constructor
    R3BNeulandDigitizer& operator=(const R3BNeulandDigitizer&) = delete; // copy assignment
    R3BNeulandDigitizer& operator=(R3BNeulandDigitizer&&) = delete;      // move assignment

  protected:
    InitStatus Init() override;
    void Finish() override;
    void SetParContainers() override;

  public:
    void Exec(Option_t* /*option*/) override;
    void SetEngine(std::unique_ptr<Digitizing::DigitizingEngineInterface> engine);
    void AddFilter(const Filterable<R3BNeulandHit&>::Filter& filter) { fHitFilters.Add(filter); }
    void AddFilterCal(const Filterable<R3B::Neuland::SimCalData&>::Filter& filter) { fCalHitFilters.Add(filter); }

    // Paula:Flag for CalData
    void EnableCalDataOutput(bool calc_cal) { is_cal_output_ = calc_cal; }
    auto HasCalDataOutput() -> bool { return is_cal_output_; }

  private:
    // Paula:Flag for CalData
    bool is_cal_output_ = false;

    R3B::InputVectorConnector<R3BNeulandPoint> fPoints{ "NeulandPoints" };
    R3B::OutputVectorConnector<R3BNeulandHit> fHits{ "NeulandHits" };
    // TCAOutputConnector<R3B::Neuland::SimCalData> fCalHits{ "NeulandSimCal" };
    R3B::OutputVectorConnector<R3B::Neuland::SimCalData> fCalHits{ "NeulandSimCal" };

    std::unique_ptr<Digitizing::DigitizingEngineInterface> fDigitizingEngine; // owning

    Filterable<R3BNeulandHit&> fHitFilters;
    Filterable<R3B::Neuland::SimCalData&> fCalHitFilters;

    R3BNeulandGeoPar* fNeulandGeoPar = nullptr; // non-owning

    TH1I* hMultOne = nullptr;
    TH1I* hMultTwo = nullptr;
    TH1F* hRLTimeToTrig = nullptr;

    void fill_cal_data(std::map<int, std::unique_ptr<R3B::Digitizing::Paddle>> paddles);

  public:
    template <typename... Args>
    explicit R3BNeulandDigitizer(Options option, Args&&... args)
        : R3BNeulandDigitizer()
    {
        switch (option)
        {
            case Options::neulandTamex:
                fDigitizingEngine = Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(),
                                                             UseChannel<TamexChannel>(std::forward<Args>(args)...));
                break;
            case Options::neulandTacquila:
                fDigitizingEngine = Digitizing::CreateEngine(UsePaddle<NeulandPaddle>(),
                                                             UseChannel<TacquilaChannel>(std::forward<Args>(args)...));
                break;
        }
    }
    ClassDefOverride(R3BNeulandDigitizer, 1) // NOLINT
};
