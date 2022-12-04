cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Werror -Wall -Wextra -Wpedantic -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -fsanitize=address,undefined -fno-sanitize-recover=all")

set(SOURCE_FILES Parser.cpp Parser.h bmp_reader.cpp bmp_reader.h exceptions.h filters.cpp filters.h crop.cpp crop.h gray_scale.cpp gray_scale.h negative.cpp negative.h sharpening.cpp sharpening.h edge_detection.cpp edge_detection.h gaussian_blur.cpp gaussian_blur.h bilateral.cpp bilateral.h)
add_executable(image_processor ${SOURCE_FILES} image_processor.cpp)

target_include_directories(image_processor PRIVATE sources)

set(CMAKE_PROJECT_ROOT "${CMAKE_SOURCE_DIR}/../../")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_PROJECT_ROOT}/cmake")
include(${CMAKE_PROJECT_ROOT}/cmake/TestSolution.cmake)
find_package(Catch REQUIRED)
