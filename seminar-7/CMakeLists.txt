cmake_minimum_required(VERSION 3.5)

project(dataset_analysis)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Werror=return-type")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

add_library(dataset_utils_lib
    OBJECT
        src/dataset_utils.cpp
        inc/dataset_utils.hpp
)

target_include_directories(dataset_utils_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dataset_utils_lib)

if (OpenMP_CXX_FOUND)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_link_libraries(dataset_utils_lib PUBLIC OpenMP::OpenMP_CXX)
endif()

option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(unit_tests tests/test_dataset_utils.cpp)

    target_link_libraries(unit_tests PRIVATE dataset_utils_lib Catch2::Catch2WithMain)

    if (OpenMP_CXX_FOUND)
        target_link_libraries(unit_tests PRIVATE OpenMP::OpenMP_CXX)
    endif()
    add_test(NAME DatasetUtilsTests COMMAND unit_tests)

    add_custom_target(run-tests
      COMMAND $<TARGET_FILE:unit_tests>
      DEPENDS unit_tests
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()

