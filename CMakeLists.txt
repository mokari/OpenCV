cmake_minimum_required(VERSION 2.8)
project( view )
set (CMAKE_CXX_STANDARD 14)
find_package( OpenCV REQUIRED )
add_executable( view Main.cpp )
target_link_libraries( view ${OpenCV_LIBS} )
