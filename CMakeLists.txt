cmake_minimum_required(VERSION 3.5)
project(astra_depth_recorder CXX)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV 4 REQUIRED)

link_directories($ENV{ASTRA_SDK_LIB})
include_directories($ENV{ASTRA_SDK_INCLUDE} ${OpenCV_INCLUDE_DIRS})

set(TARGET_NAME "astra_depth_recorder")
add_executable(${TARGET_NAME} astra_depth_recorder.cpp)

target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS}  astra astra_core )
target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic)
