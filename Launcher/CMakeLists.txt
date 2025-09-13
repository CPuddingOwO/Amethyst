cmake_minimum_required(VERSION 3.30)
project(Launcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Launcher main.cpp)
target_link_libraries(Launcher PRIVATE Amethyst)