cmake_minimum_required(VERSION 3.12)
project(chess-gui)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)


find_package(GTest CONFIG REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)   

add_executable(tests BoardGUI/board.cpp BoardGUI/piece.cpp tests/test_fenToBoard.cpp tests/test_move_calculation.cpp)

target_link_libraries(tests GTest::gtest_main)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic -Werror)   
target_link_libraries(tests sfml-graphics sfml-window sfml-system)

