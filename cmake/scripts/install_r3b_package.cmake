include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(CMakeModulesDir ${CMAKE_INSTALL_LIBDIR}/cmake/R3BRoot)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/scripts/R3BRootConfig.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/scripts/R3BRootConfig.cmake
    INSTALL_DESTINATION ${CMakeModulesDir}
    PATH_VARS CMakeModulesDir
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

install(
    EXPORT R3BRoot
    DESTINATION ${CMakeModulesDir}
    NAMESPACE R3BRoot::
    FILE R3BRootTargets.cmake)

install(FILES ${CMAKE_BINARY_DIR}/cmake/scripts/R3BRootConfig.cmake DESTINATION ${CMakeModulesDir})
