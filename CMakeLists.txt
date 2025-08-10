cmake_minimum_required(VERSION 3.28)
project(Skyline)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Skyline main.cpp)

target_link_libraries(Skyline OpenGL::GL GLUT::GLUT)