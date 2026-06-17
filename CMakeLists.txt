cmake_minimum_required(VERSION 3.16)

project(TicTacToeWithDoctest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

add_library(tic_tac_toe_lib
    src/tic_tac_toe.cpp
)

target_include_directories(tic_tac_toe_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(tic_tac_toe
    src/main.cpp
)

target_link_libraries(tic_tac_toe
    PRIVATE
        tic_tac_toe_lib
)

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        doctest
        URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.11.zip
    )

    FetchContent_MakeAvailable(doctest)

    add_executable(tic_tac_toe_tests
        tests/test_tic_tac_toe.cpp
    )

    target_link_libraries(tic_tac_toe_tests
        PRIVATE
            tic_tac_toe_lib
            doctest::doctest
    )

    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

    doctest_discover_tests(tic_tac_toe_tests)
endif()