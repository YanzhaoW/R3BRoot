project(R3BYMLConfig)
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE yaml-cpp R3BCppReflection)
target_include_directories(${PROJECT_NAME} INTERFACE .)
