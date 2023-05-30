##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019-2023 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

set(PROJECT_NAME R3BNeulandHitParameter)

set(LIBRARY_NAME ${PROJECT_NAME})
set(LINKDEF NeulandHitParameterLinkDef.h)

# set(DEPENDENCIES R3BBase R3BData R3BTCal R3BNeulandShared)
set(DEPENDENCIES R3BBase R3BData R3BNeulandShared)

set(INCLUDE_DIRECTORIES
    ${INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    # ${R3BROOT_SOURCE_DIR}/neuland/calibration
    # ${R3BROOT_SOURCE_DIR}/neuland/calibration/hitParameter
    # ${R3BROOT_SOURCE_DIR}/neuland/shared
    # ${R3BROOT_SOURCE_DIR}/r3bdata/losData ${R3BROOT_SOURCE_DIR}/tcal
    )

include_directories(${INCLUDE_DIRECTORIES})

set(SRCS
    R3BNeulandCosmicTracker.cxx
    R3BNeulandHitCalibrationBar.cxx
    R3BNeulandHitCalibrationEngine.cxx
    R3BNeulandHitModulePar.cxx
    R3BNeulandHitPar.cxx
    LSQR.cxx
    R3BNeulandTSyncer.cxx
    )
change_file_extension(*.cxx *.h HEADERS "${SRCS}")
generate_library()
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
