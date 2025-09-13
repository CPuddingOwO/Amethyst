cmake_minimum_required(VERSION 3.31)
project(Amethyst LANGUAGES CXX)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/bin")
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# ImGui
set(IMGUI_ENABLE_SDL3 ON)
set(IMGUI_ENABLE_SDLRENDERER3 ON)

# SDL3
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# add_subdirectory()
add_subdirectory(external/sdl3)
add_subdirectory(Logging)
# find_package()
find_package(LibreSSL)
find_package(httplib)
find_package(spdlog)
find_package(nlohmann_json)
#find_package(cryptopp)

set(AmethystSources
        ${CMAKE_SOURCE_DIR}/src/Amethyst.cpp

        # ImGui
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdlrenderer3.cpp

        inc/Amethyst/Module/BiliHelper/Constants.hpp
        inc/Amethyst/Module/BiliHelper/Network/Client.hpp
        src/Module/BiliHelper/Network/Client.cpp
        inc/Amethyst/Compat/KDABNlomannCompat.hpp
        inc/Amethyst/Prelude/json.hpp
        inc/Amethyst/Storage.hpp
        inc/Amethyst/Module/BiliHelper/Network/RequestTypes.hpp
        inc/Amethyst/Module/BiliHelper/Network/ResponseTypes.hpp
        inc/Amethyst/Module/BiliHelper/Ui/BiliLive.hpp
        inc/Amethyst/SimpleUI.hpp
        inc/Amethyst/Module/BiliHelper/UserManager.hpp
        inc/Amethyst/Module/BiliHelper/SimpleTypes.hpp
        src/Storage.cpp
        src/Module/BiliHelper/UserManager.cpp
        inc/Amethyst/Module/BiliHelper/Network/WbiCheck.hpp
)

add_library(Amethyst STATIC ${AmethystSources})
target_include_directories(Amethyst PUBLIC inc
        external/imgui
        external/imgui/backends
        external/imgui/misc
)
target_compile_definitions(Amethyst PRIVATE
        CPPHTTPLIB_OPENSSL_SUPPORT
)
target_link_libraries(Amethyst PUBLIC
        spdlog::spdlog
        Logging
        SDL3::SDL3-static
        libressl::libressl
        httplib::httplib
        nlohmann_json::nlohmann_json
#        cryptopp::cryptopp
)


enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(test)      # Test
add_subdirectory(Launcher)   # App Launcher