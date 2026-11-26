# Install ######################################################################
include(CMakePackageConfigHelpers)

set(cmakeModulesDir cmake)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/scripts/R3BRootConfig.cmake.in
    ${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS cmakeModulesDir
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
    EXPORT R3BRootLibs
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Config-targets.cmake
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/check_system.sh
        ${CMAKE_CURRENT_BINARY_DIR}/check_system.csh
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)


install(
    DIRECTORY 
        ${PROJECT_SOURCE_DIR}/geometry
        ${PROJECT_SOURCE_DIR}/gconfig
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
)

install(
    FILES 
        ${PROJECT_SOURCE_DIR}/config/rootmanager.dat
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/config"
)

