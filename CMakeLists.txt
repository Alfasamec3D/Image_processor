cmake_minimum_required(VERSION 3.5)

project(image_proc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5  REQUIRED COMPONENTS Widgets Gui)
include_directories(${OpenCV_INCLUDE_DIRS} include)
enable_testing()
add_library(proc src/proc.cpp include/proc.hpp)
add_executable(image_proc image_proc.cpp)
target_link_libraries(proc Qt5::Gui Qt5::Widgets)

target_link_libraries(image_proc ${OpenCV_LIBS} proc  Qt5::Widgets Qt5::Gui)