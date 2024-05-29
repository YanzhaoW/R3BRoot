# Note the Fortran project mention in the main CMakeLists.txt.
include(CheckFortran)
add_library(R3BTraRene SHARED tracker_routines.f95)
add_library(R3B::TraRene ALIAS R3BTraRene)
target_include_directories(R3BTraRene PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
install(
    TARGETS R3BTraRene
    EXPORT R3BRoot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
