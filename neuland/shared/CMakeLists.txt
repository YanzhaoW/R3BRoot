##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019-2024 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

set(SRCS
    ElasticScattering.cxx
    IsElastic.cxx
    R3BNeulandGeoPar.cxx
    R3BNeulandContFact.cxx
    R3BProgramOptions.cxx
    R3BNeulandVisualizer.cxx)

set(HEADERS
    ClusteringEngine.h
    ElasticScattering.h
    Filterable.h
    TCAConnector.h
    Validated.h
    IsElastic.h
    R3BProgramOptions.h
    R3BNeulandGeoPar.h
    R3BNeulandContFact.h
    R3BNeulandVisualizer.h
    R3BNeulandCommon.h)

# find_package(Protobuf REQUIRED) protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS neuland.proto)
# protobuf_generate_python(PROTO_PY neuland.proto) set(SRCS ${SRCS} ${PROTO_SRCS}) set(HEADERS
# ${HEADERS} ${PROTO_HDRS}) Include directory such that protoc generated headers are found
# include_directories(${CMAKE_CURRENT_BINARY_DIR}) ${PROTOBUF_LIBRARY}

add_library_with_dictionary(
    LIBNAME
    R3BNeulandShared
    LINKDEF
    NeulandSharedLinkDef.h
    HEADERS
    ${HEADERS}
    SRCS
    ${SRCS}
    INCLUDEDIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDENCIES
    R3BData
    Boost::program_options)
