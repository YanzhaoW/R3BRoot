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

// ------------------------------------------------------------------
// -----              R3BFootCalPar source file                 -----
// -----         Created 07/09/21 by J.L. Rodriguez-Sanchez     -----
// ------------------------------------------------------------------

#include "FairLogger.h"
#include "FairParamList.h"

#include "R3BFootCalPar.h"

#include "TArrayF.h"
#include "TMath.h"
#include "TString.h"
#include <iostream>

// ---- Standard Constructor ---------------------------------------------------
R3BFootCalPar::R3BFootCalPar(const char* name, const char* title, const char* context)
    : FairParGenericSet(name, title, context)
{
    detName = "FootCal";
    fNumDets = 8;
    fNumStrips = 640;
    fNumParsFit = 2;
    fStripCalParams = new TArrayF(fNumDets * fNumStrips * fNumParsFit);
}

// ----  Destructor ------------------------------------------------------------
R3BFootCalPar::~R3BFootCalPar()
{
    clear();
    delete fStripCalParams;
}

// ----  Method clear ----------------------------------------------------------
void R3BFootCalPar::clear()
{
    status = kFALSE;
    resetInputVersions();
}

// ----  Method putParams ------------------------------------------------------
void R3BFootCalPar::putParams(FairParamList* list)
{
    LOG(INFO) << "R3BFootCalPar::putParams() called";
    if (!list)
    {
        return;
    }

    Int_t array_size = fNumDets * fNumStrips;
    LOG(INFO) << "Array Size: " << array_size;

    fStripCalParams->Set(array_size);

    list->add("footDetNumberPar", fNumDets);
    list->add("footStripNumberPar", fNumStrips);
    list->add("footNumberParsFit", fNumParsFit);
    list->add("footStripCalPar", *fStripCalParams);
}

// ----  Method getParams ------------------------------------------------------
Bool_t R3BFootCalPar::getParams(FairParamList* list)
{
    LOG(INFO) << "R3BFootCalPar::getParams() called";
    if (!list)
    {
        return kFALSE;
    }

    if (!list->fill("footDetNumberPar", &fNumDets))
    {
        return kFALSE;
    }

    if (!list->fill("footStripNumberPar", &fNumStrips))
    {
        return kFALSE;
    }

    if (!list->fill("footNumberParsFit", &fNumParsFit))
    {
        return kFALSE;
    }

    Int_t array_size = fNumDets * fNumStrips;
    LOG(INFO) << "Total number of strips: " << array_size;
    fStripCalParams->Set(array_size * fNumParsFit);

    if (!(list->fill("footStripCalPar", fStripCalParams)))
    {
        LOG(ERROR) << "R3BFootCalPar:Could not initialize footStripCalPar";
        return kFALSE;
    }

    return kTRUE;
}

// ----  Method print ----------------------------------------------------------
void R3BFootCalPar::print()
{
    LOG(INFO) << "R3BFootCalPar:Foot strip Parameters: ";
    Int_t array_size = fNumDets * fNumStrips;

    for (Int_t d = 0; d < fNumDets; d++)
    {
        LOG(INFO) << "Foot detector number: " << d;
        for (Int_t i = 0; i < fNumStrips; i++)
        {
            LOG(INFO) << "Foot strip number: " << i;
            for (Int_t j = 0; j < fNumParsFit; j++)
            {
                LOG(INFO) << "FitParam(" << j
                          << ") = " << fStripCalParams->GetAt(d * fNumParsFit * fNumStrips + i * fNumParsFit + j);
            }
        }
    }
}

ClassImp(R3BFootCalPar);
