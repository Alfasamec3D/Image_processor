cmake_minimum_required(VERSION 3.5)

project(image_proc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5  REQUIRED COMPONENTS Widgets Gui)

include_directories(${OpenCV_INCLUDE_DIRS} include)
enable_testing()
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)
FetchContent_MakeAvailable(googletest)

add_library(gui src/gui.cpp include/gui.hpp)
add_library(proc src/proc.cpp)

add_executable(image_proc image_proc.cpp)
add_executable(testgrayscale tests/testGrayscale.cpp)
add_executable(testgrayscalesection tests/testGrayscaleSection.cpp)
add_executable(testblur tests/testBlur.cpp)
add_executable(testblursection tests/testBlurSection.cpp)

target_link_libraries(gui Qt5::Gui Qt5::Widgets)
target_link_libraries (gui proc)
target_link_libraries (proc ${OpenCV_LIBS})
target_link_libraries(testgrayscale proc gtest_main)
target_link_libraries(testgrayscalesection proc gtest_main)
target_link_libraries(testblursection proc gtest_main)
target_link_libraries(testblur proc gtest_main)
target_link_libraries(image_proc proc gui)

include(GoogleTest)
gtest_discover_tests(testgrayscale)
gtest_discover_tests(testgrayscalesection)
gtest_discover_tests(testblursection)
gtest_discover_tests(testblur)