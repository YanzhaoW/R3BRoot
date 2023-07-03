project(R3BMultiProc)
add_library(${PROJECT_NAME} R3BMultiProcess.cxx )
target_link_libraries(${PROJECT_NAME} PUBLIC R3BNeulandShared)
target_include_directories(${PROJECT_NAME} PUBLIC .)
