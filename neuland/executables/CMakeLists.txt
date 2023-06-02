set(EXE_NAME neulandRun)
set(DEPENDENCIES
    R3BNeulandShared
    R3BNeulandSimulation
    R3BNeulandDigitizing
    R3BPassive
    R3BData
    R3BBase
    Gen
    ${Geant4_LIBRARIES}
    ${Geant4VMC_LIBRARIES}
    ExTestDetector
    Boost::program_options)

set(INCLUDE_DIRECTORIES
    ${INCLUDE_DIRECTORIES} ${R3BROOT_SOURCE_DIR}/passive ${R3BROOT_SOURCE_DIR}/neuland/executables
    ${R3BROOT_SOURCE_DIR}/neuland/simulation ${Geant4_INCLUDE_DIRS} ${Geant4VMC_INCLUDE_DIRS})

message(STATUS "include dir: ${Geant4_INCLUDE_DIRS}")
include_directories(${INCLUDE_DIRECTORIES})
set(SRCS neulandRun.cxx R3BYamlDetectors.cxx R3BNeulandSimulator.cxx R3BNeulandRunner.cxx
    R3BNeulandAnalyser.cxx R3BYamlTasks.cxx)

generate_executable()
