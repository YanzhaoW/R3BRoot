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

// -----------------------------------------------------------------
// -----         R3BFootMapped2StripCal source file            -----
// -----      Created 08/09/21 by J.L. Rodriguez-Sanchez       -----
// -----------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// AMS headers
#include "R3BFootCalData.h"
#include "R3BFootCalPar.h"
#include "R3BFootMapped2StripCal.h"
#include "R3BFootMappedData.h"

// R3BFootMapped2StripCal: Default Constructor --------------------------
R3BFootMapped2StripCal::R3BFootMapped2StripCal()
    : R3BFootMapped2StripCal("R3B FOOT Calibrator", 1)
{
}

// R3BFootMapped2StripCalPar: Standard Constructor --------------------------
R3BFootMapped2StripCal::R3BFootMapped2StripCal(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , NumDets(1)
    , NumStrips(640)
    , NumParams(2)
    , MaxSigma(5)
    , fTimesSigma(3.)
    , CalParams(NULL)
    , fCal_Par(NULL)
    , fFootMappedData(NULL)
    , fFootCalData(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BFootMapped2StripCal: Destructor
R3BFootMapped2StripCal::~R3BFootMapped2StripCal()
{
    LOG(DEBUG) << "R3BFootMapped2StripCal: Delete instance";
    if (fFootMappedData)
        delete fFootMappedData;
    if (fFootCalData)
        delete fFootCalData;
}

void R3BFootMapped2StripCal::SetParContainers()
{
    // Parameter Container
    // Reading footCalPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fCal_Par = (R3BFootCalPar*)rtdb->getContainer("footCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BFootMapped2StripCalPar::SetParContainers() footCalPar container not found";
    }
    else
    {
        LOG(INFO) << "R3BFootMapped2StripCalPar::SetParContainers() footCalPar container open";
    }
}

void R3BFootMapped2StripCal::SetParameter()
{
    //--- Parameter Container ---
    NumDets = fCal_Par->GetNumDets();      // Number of Detectors
    NumStrips = fCal_Par->GetNumStrips();  // Number of Strips
    NumParams = fCal_Par->GetNumParsFit(); // Number of Parameters

    LOG(INFO) << "R3BFootMapped2StripCal: Nb detectors: " << NumDets;
    LOG(INFO) << "R3BFootMapped2StripCal: Nb strips: " << NumStrips;
    LOG(INFO) << "R3BFootMapped2StripCal: Nb parameters from pedestal fit: " << NumParams;

    CalParams = new TArrayF();
    Int_t array_size = NumDets * NumStrips * NumParams;
    CalParams->Set(array_size);
    CalParams = fCal_Par->GetStripCalParams(); // Array with the Cal parameters

    // Count the number of dead strips per AMS detector
    for (Int_t d = 0; d < NumDets; d++)
    {
        Int_t numdeadstrips = 0;
        for (Int_t i = 0; i < NumStrips; i++)
        {
            if (CalParams->GetAt(NumParams * i + 1 + NumStrips * d * NumParams) == -1)
                numdeadstrips++;
        }
        LOG(INFO) << "R3BFootMapped2StripCal: Nb of dead strips in FOOT detector " << d << ": " << numdeadstrips;
    }
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BFootMapped2StripCal::Init()
{
    LOG(INFO) << "R3BFootMapped2StripCal::Init()";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fFootMappedData = (TClonesArray*)rootManager->GetObject("FootMappedData");
    if (!fFootMappedData)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    // Calibrated data
    fFootCalData = new TClonesArray("R3BFootCalData", 10);
    rootManager->Register("FootCalData", "FOOT strip Cal", fFootCalData, !fOnline);

    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BFootMapped2StripCal::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BFootMapped2StripCal::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Reading the Input -- Mapped Data --
    Int_t nHits = fFootMappedData->GetEntries();
    if (nHits != NumStrips * NumDets && nHits > 0)
        LOG(WARNING) << "R3BFootMapped2StripCal: nHits!=NumStrips*NumDets";
    if (!nHits)
        return;

    R3BFootMappedData** mappedData = new R3BFootMappedData*[nHits];
    Int_t detId;
    Int_t stripId;
    Double_t energy;
    Double_t pedestal = 0.;
    Double_t sigma = 0.;

    Int_t nbadc = 0;
    Int_t minnb = 4;
    Double_t h = 0;

    nbadc = 0;
    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BFootMappedData*)(fFootMappedData->At(i));
        detId = mappedData[i]->GetDetId();
        stripId = mappedData[i]->GetStripId();

        pedestal = CalParams->GetAt(NumParams * stripId + detId * NumParams * NumStrips);
        sigma = CalParams->GetAt(NumParams * stripId + 1 + detId * NumParams * NumStrips);

        energy = mappedData[i]->GetEnergy() - pedestal - fTimesSigma * sigma;

        // We accept the hit if the energy is larger than 5 times the sigma of the pedestal
        // and the strip is not dead
        if (energy > 0. && pedestal != -1)
        {
            AddCalData(detId, stripId, energy);
        }
    }

    if (mappedData)
        delete mappedData;
    return;
}

// -----   Public method Reset   ------------------------------------------------
void R3BFootMapped2StripCal::Reset()
{
    LOG(DEBUG) << "Clearing StripCalData Structure";
    if (fFootCalData)
        fFootCalData->Clear();
}

// -----   Private method AddCalData  --------------------------------------------
R3BFootCalData* R3BFootMapped2StripCal::AddCalData(Int_t detid, Int_t stripid, Double_t energy)
{
    // It fills the R3BFootCalData
    TClonesArray& clref = *fFootCalData;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BFootCalData(detid, stripid, energy);
}

ClassImp(R3BFootMapped2StripCal);
