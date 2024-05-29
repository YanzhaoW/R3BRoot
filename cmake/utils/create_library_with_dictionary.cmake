# format build include dirs for installations
function(format_build_include_dirs list_dirs out_dirs)
    set(formatted_list_dirs)
    foreach(dir ${list_dirs})
        if(NOT IS_ABSOLUTE ${dir})
            set(dir ${CMAKE_CURRENT_SOURCE_DIR}/${dir})
        endif()
        list(APPEND formatted_list_dirs $<BUILD_INTERFACE:${dir}>)
    endforeach()
    set(${out_dirs}
        ${formatted_list_dirs}
        PARENT_SCOPE)
endfunction()

# generate root dictioanry from cmake target
function(add_library_with_dictionary)
    set(options)
    set(oneValueArgs LIBNAME LINKDEF)
    set(multiValueArgs
        SRCS
        INCLUDEDIRS
        HEADERS
        PRIVATE_INCLUDEDIRS
        DEPENDENCIES
        PRIVATE_DEPENDENCIES)
    cmake_parse_arguments(
        ARGS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    add_library(${ARGS_LIBNAME} SHARED)

    add_library(R3B::${ARGS_LIBNAME} ALIAS ${ARGS_LIBNAME})

    target_sources(
        ${ARGS_LIBNAME}
        PRIVATE ${ARGS_SRCS}
        PUBLIC FILE_SET
               HEADERS
               BASE_DIRS
               ${CMAKE_CURRENT_SOURCE_DIR}
               FILES
               ${ARGS_HEADERS})

    format_build_include_dirs("${ARGS_INCLUDEDIRS}" ARGS_INCLUDEDIRS)
    target_include_directories(
        ${ARGS_LIBNAME}
        PUBLIC ${ARGS_INCLUDEDIRS}
        PRIVATE ${ARGS_PRIVATE_INCLUDEDIRS})

    target_link_libraries(
        ${ARGS_LIBNAME}
        PUBLIC ${ARGS_DEPENDENCIES}
        PRIVATE ${ARGS_PRIVATE_DEPENDENCIES})

    root_generate_dictionary(
        ${ARGS_LIBNAME}_dict
        ${ARGS_HEADERS}
        MODULE
        ${ARGS_LIBNAME}
        LINKDEF
        ${ARGS_LINKDEF})

    install(
        TARGETS ${ARGS_LIBNAME}
        EXPORT R3BRoot
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} FILE_SET HEADERS
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endfunction()

# generate macros using availble variables. Please use create_library_with_dictionary instead.
macro(ADD_LIB_WITH_DIC)
    add_library_with_dictionary(
        LIBNAME
        ${LIBRARY_NAME}
        LINKDEF
        ${LINKDEF}
        HEADERS
        ${HEADERS}
        SRCS
        ${SRCS}
        INCLUDEDIRS
        ${INCLUDE_DIRS}
        DEPENDENCIES
        ${DEPENDENCIES})
endmacro()
