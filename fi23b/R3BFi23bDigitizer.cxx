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

#include "R3BFi23bDigitizer.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TClonesArray.h"

// includes for modeling
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMatrix.h"
#include "TGeoMedium.h"
#include "TGeoShapeAssembly.h"
#include "TParticle.h"
#include "TVirtualMC.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "R3BFibPoint.h"
#include "R3BMCTrack.h"

using std::cout;
using std::endl;

R3BFi23bDigitizer::R3BFi23bDigitizer()
    : FairTask("R3B Fi23b Digitization scheme ")
    , fFi23bPoints(NULL)
{

    esigma = 0.001;
    tsigma = 0.01;
    ysigma = 1;
}

R3BFi23bDigitizer::R3BFi23bDigitizer(Double_t e, Double_t t, Double_t y)
    : FairTask("R3B Fi23b Digitization scheme ")
    , fFi23bPoints(NULL)
{

    esigma = e;
    tsigma = t;
    ysigma = y;
}

R3BFi23bDigitizer::~R3BFi23bDigitizer() {}

void R3BFi23bDigitizer::SetEnergyResolution(Double_t e) { esigma = e; }

void R3BFi23bDigitizer::SetTimeResolution(Double_t t) { tsigma = t; }

void R3BFi23bDigitizer::SetYPositionResolution(Double_t y) { ysigma = y; }

InitStatus R3BFi23bDigitizer::Init()
{

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
        LOG(fatal) << "Init:No FairRootManager";
    fFi23bPoints = (TClonesArray*)ioman->GetObject("Fi23bPoint");

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");

    // Register output array DchDigi
    fFi23bHits = new TClonesArray("R3BBunchedFiberHitData", 1000);
    ioman->Register("Fi23bHit", "Digital response in Fi23b", fFi23bHits, kTRUE);

    // for sigmas
    prnd = new TRandom3();

    return kSUCCESS;
}

void R3BFi23bDigitizer::Exec(Option_t* opt)
{
    Reset();

    auto Digitize = [this](TClonesArray* Points, TClonesArray* Hits, Int_t NumOfFibers) {
        Int_t entryNum = Points->GetEntries();

        if (!entryNum)
            return;

        // creating the storage for energy and time for each PMT
        std::vector<Double_t>* energy = new std::vector<Double_t>[NumOfFibers];
        std::vector<Double_t>* time = new std::vector<Double_t>[NumOfFibers];
        std::vector<Double_t>* y = new std::vector<Double_t>[NumOfFibers];

        for (Int_t i = 0; i < NumOfFibers; ++i)
        {
            time[i].push_back(99999);
            energy[i].push_back(0);
            y[i].push_back(0);
        }

        struct TempHit
        {
            Int_t fiberID;
            Double_t Energy;
            Double_t Time;
            Double_t Y;

            TempHit(Int_t id, Double_t e, Double_t t, Double_t y)
                : fiberID(id)
                , Energy(e)
                , Time(t)
                , Y(y)
            {
            }
        };

        // ordering the hits in time

        std::vector<TempHit> TempHits;

        for (Int_t i = 0; i < entryNum; ++i)
        {
            R3BFibPoint* data_element = (R3BFibPoint*)Points->At(i);

            TempHits.push_back(TempHit(data_element->GetDetectorID(),
                                       data_element->GetEnergyLoss(),
                                       data_element->GetTime(),
                                       data_element->GetYIn()));
        }

        std::sort(TempHits.begin(), TempHits.end(), [](const TempHit& lhs, const TempHit& rhs) {
            return lhs.Time < rhs.Time;
        });

        // registering the hits in the PMTs

        for (TempHit& Hit : TempHits)
        {
            if (Hit.Energy < 0.0001)
            {
                continue;
            }

            Int_t fiberID = Hit.fiberID;

            if (Hit.Time - time[fiberID].back() < 30)
            {
                energy[fiberID].back() += Hit.Energy;

                y[fiberID].back() = (time[fiberID].back() > Hit.Time) ? Hit.Y : y[fiberID].back();

                time[fiberID].back() = (time[fiberID].back() > Hit.Time) ? Hit.Time : time[fiberID].back();
            }
            else
            {
                energy[fiberID].push_back(Hit.Energy);
                time[fiberID].push_back(Hit.Time);
                y[fiberID].push_back(Hit.Y);
            }
        }

        // creating the final hits

        for (Int_t i = 0; i < NumOfFibers; ++i)
        {
            for (Double_t& energyl : energy[i])
            {
                if (energyl > 0.0001)
                {


                    Double_t fiber_id = i;
                    Double_t xpos = -detector_width / 2. + fiber_thickness * (1 + air_layer) / 2. +
                                    fiber_id * (1 + air_layer) * fiber_thickness;

                    // cout<<setprecision(10) << "Test Det = 23b" << " x = " << xpos << " fiber = " << i << endl;


                    new ((*Hits)[Hits->GetEntries()])
                        R3BBunchedFiberHitData(2,
                                               xpos,
                                               prnd->Gaus((y[i].at(&energyl - energy[i].data())), ysigma),
                                               prnd->Gaus(energyl, esigma),
                                               prnd->Gaus(time[i].at(&energyl - energy[i].data()), tsigma),
                                               i,
                                               0.,
                                               0.,
                                               0.,
                                               0.);
                }
            }
        }

        delete[] energy;
        delete[] time;
        delete[] y;
    };

    // running the digitizer for the Fi detectors

    if (fFi23bPoints)
    {
        Digitize(fFi23bPoints, fFi23bHits, fiber_nbr);
    }
}
// -------------------------------------------------------------------------

void R3BFi23bDigitizer::Reset()
{
    if (fFi23bHits)
        fFi23bHits->Clear();
}

void R3BFi23bDigitizer::Finish() {}

ClassImp(R3BFi23bDigitizer)
