cmake_minimum_required(VERSION 3.30)
project(13286530_IPA_A1)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 23)

add_executable(13286530_IPA_A1 main.cpp)

target_link_libraries( 13286530_IPA_A1 ${OpenCV_LIBS} )
