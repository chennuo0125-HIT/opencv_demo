cmake_minimum_required(VERSION 2.8.3)
project(calibrator)
add_compile_options(-std=c++11)

set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(calibrator src/camera_calibration.cpp)
target_link_libraries(calibrator ${OpenCV_LIBRARIES})