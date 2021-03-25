cmake_minimum_required(VERSION 2.8.3)
project(aruco)
add_compile_options(-std=c++11)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(slam_orb src/ORBextractor.cc)
target_link_libraries(slam_orb ${OpenCV_LIBRARIES})

add_executable(orb_compare src/orb_compare.cc)
target_link_libraries(orb_compare slam_orb)