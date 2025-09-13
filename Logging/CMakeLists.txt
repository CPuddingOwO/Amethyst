cmake_minimum_required(VERSION 3.30)
project(Logging LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)

set(LoggingSources
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Logging/log.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib.cpp
)

add_library(Logging STATIC ${LoggingSources})
target_include_directories(Logging PUBLIC include)
# Conan
target_link_libraries(Logging PUBLIC spdlog::spdlog)