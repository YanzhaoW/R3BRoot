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

#########################################################
# temporary workaround for testR3BPhaseSpaceGenerator.cxx
# while FairVersion.h is not working
execute_process(COMMAND ${FAIRROOTPATH}/bin/fairroot-config --major_version
                OUTPUT_VARIABLE fairroot_major_version)
execute_process(COMMAND ${FAIRROOTPATH}/bin/fairroot-config --minor_version
                OUTPUT_VARIABLE fairroot_minor_version)

if((${fairroot_major_version} EQUAL 17 AND ${fairroot_minor_version} EQUAL 10)
   OR ${fairroot_major_version} GREATER 17)
    message("using new FairPrimaryGenerator interface")
    add_definitions(-DFairPrimaryGeneratorAddTrackNewInterface)
endif((${fairroot_major_version} EQUAL 17 AND ${fairroot_minor_version} EQUAL 10)
      OR ${fairroot_major_version} GREATER 17)
#########################################################

# set(SYSTEM_INCLUDE_DIRECTORIES ${SYSTEM_INCLUDE_DIRECTORIES} ${BASE_INCLUDE_DIRECTORIES})
#
# set(INCLUDE_DIRECTORIES
#     ${R3BROOT_SOURCE_DIR}/field 
#     ${R3BROOT_SOURCE_DIR}/generators 
#     ${R3BROOT_SOURCE_DIR}/r3bgen
#     ${R3BROOT_SOURCE_DIR}/r3bdata 
#     ${R3BROOT_SOURCE_DIR}/r3bbase 
#     ${Geant4_INCLUDE_DIRS}
# )
#
# include_directories(${INCLUDE_DIRECTORIES})
# include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

# set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR} ${FairLogger_LIBDIR} ${FAIRROOT_LIBRARY_DIR}
#                      ${Geant4_LIBRARY_DIR})

# link_directories(${LINK_DIRECTORIES})

# set(SRCS
#     R3BSpecificGenerator.cxx
#     R3BReadKinematics.cxx
#     R3BCDGenerator.cxx
#     R3BBeamInfo.cxx
#     R3BBackTracking.cxx
#     R3BBackTrackingStorageState.cxx
#     R3BAsciiGenerator.cxx
#     R3BCosmicGenerator.cxx
#     R3BCryAsciiGenerator.cxx
#     R3Bp2pGenerator.cxx
#     R3BLandGenerator.cxx
#     R3BCALIFATestGenerator.cxx
#     R3BParticleGenerator.cxx
#     R3BIonGenerator.cxx
#     R3BGammaGenerator.cxx
#     R3BPhaseSpaceGenerator.cxx
#     R3BDistribution1D.cxx
#     R3BDistribution2D.cxx
#     R3BDistribution3D.cxx
#     R3Bp2pevtGenerator.cxx
#     R3BParticleSelector.cxx
#     R3BBeamProperties.cxx
#     R3BINCLRootGenerator.cxx
#     )

# fill list of header files from list of source files
# by exchanging the file extension
# change_file_extension(*.cxx *.h HEADERS "${SRCS}")

# set(LINKDEF R3BGenLinkDef.h)
# set(LIBRARY_NAME R3BGen)
# set(DEPENDENCIES
#     FairTools
#     Field
#     Gen
#     R3BBase
#     R3BData
#     G4materials
#     Boost::iostreams
#     Boost::filesystem)

# generate_library()
# target_include_directories(R3BGen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Testing
add_subdirectory(test)

set(SRCS
    R3BSpecificGenerator.cxx
    R3BReadKinematics.cxx
    R3BCDGenerator.cxx
    R3BBeamInfo.cxx
    R3BBackTracking.cxx
    R3BBackTrackingStorageState.cxx
    R3BAsciiGenerator.cxx
    R3BCosmicGenerator.cxx
    R3BCryAsciiGenerator.cxx
    R3Bp2pGenerator.cxx
    R3BLandGenerator.cxx
    R3BCALIFATestGenerator.cxx
    R3BParticleGenerator.cxx
    R3BIonGenerator.cxx
    R3BGammaGenerator.cxx
    R3BPhaseSpaceGenerator.cxx
    R3BDistribution1D.cxx
    R3BDistribution2D.cxx
    R3BDistribution3D.cxx
    R3Bp2pevtGenerator.cxx
    R3BParticleSelector.cxx
    R3BBeamProperties.cxx
    R3BINCLRootGenerator.cxx)

set(HEADERS
R3BSpecificGenerator.h
    R3BReadKinematics.h
    R3BCDGenerator.h
    R3BBeamInfo.h
    R3BBackTracking.h
    R3BBackTrackingStorageState.h
    R3BAsciiGenerator.h
    R3BCosmicGenerator.h
    R3BCryAsciiGenerator.h
    R3Bp2pGenerator.h
    R3BLandGenerator.h
    R3BCALIFATestGenerator.h
    R3BParticleGenerator.h
    R3BIonGenerator.h
    R3BGammaGenerator.h
    R3BPhaseSpaceGenerator.h
    R3BDistribution1D.h
    R3BDistribution2D.h
    R3BDistribution3D.h
    R3Bp2pevtGenerator.h
    R3BParticleSelector.h
    R3BBeamProperties.h
    R3BINCLRootGenerator.h)

add_library(R3BGen SHARED ${SRCS})
target_include_directories(R3BGen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(R3BGen PUBLIC FairTools
    Field
    Gen
    R3BBase
    R3BData
    ${Geant4_LIBRARIES}
    Boost::iostreams
    Boost::filesystem)

root_generate_dictionary(
    R3BGen_dict
    ${HEADERS}
    MODULE
    R3BGen
    LINKDEF
    R3BGenLinkDef.h)
