cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(example)

#find_package(OpenCV REQUIRED)
#include_directories("${OpenCV_INCLUDE_DIRS}")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC")

set(FACE_LIBS "/usr/lib64/libxcaan_faceengine.so")

add_definitions(-DGLIBCXX_USE_CXX11_ABI=1)

add_executable(main main.cpp)

target_link_libraries(main "${FACE_LIBS}" "-lpthread" "${OpenCV_LIBS}")

set_property(TARGET main PROPERTY CXX_STANDARD 14)

