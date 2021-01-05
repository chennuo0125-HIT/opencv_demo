cmake_minimum_required(VERSION 2.8.3)
project(aruco)
add_compile_options(-std=c++11)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(${OpenCV_INCLUDE_DIRS})

add_library(aruco src/aruco.cpp)
target_link_libraries(aruco ${OpenCV_LIBRARIES})

add_executable(generate_marker src/generate_marker_demo.cpp)
target_link_libraries(generate_marker aruco)

add_executable(estimate_pose src/estimate_pose_demo.cpp)
target_link_libraries(estimate_pose aruco)