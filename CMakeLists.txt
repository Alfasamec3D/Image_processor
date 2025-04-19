cmake_minimum_required(VERSION 3.5)

project(image_proc)

find_package(OpenCV REQUIRED)
find_package(Qt5  REQUIRED COMPONENTS Widgets Gui)
include_directories(${OpenCV_INCLUDE_DIRS})
enable_testing()
add_library(proc src/proc.cpp)
add_executable(image_proc image_proc.cpp)
target_include_directories(proc PUBLIC "include")
target_link_libraries(proc Qt5::Gui)

target_link_libraries(image_proc ${OpenCV_LIBS} proc Qt5::Widgets Qt5::Gui)