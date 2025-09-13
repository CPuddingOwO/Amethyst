# cmake/ImGui.cmake

# 允许用户通过变量选择需要编译哪些实现
option(IMGUI_ENABLE_SDL3 "Enable ImGui SDL3 implementation" OFF)
option(IMGUI_ENABLE_SDLRENDERER3 "Enable ImGui SDLRenderer3 implementation" OFF)
option(IMGUI_ENABLE_OPENGL3 "Enable ImGui OpenGL3 implementation" OFF)
# ... 你可以继续加更多

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../external/imgui")

# 基础imgui源文件
set(IMGUI_CORE_SRCS
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
)

set(IMGUI_IMPL_SRCS "")
set(IMGUI_IMPL_INCS "")

if(IMGUI_ENABLE_SDL3)
    list(APPEND IMGUI_IMPL_SRCS
            ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    )
    list(APPEND IMGUI_IMPL_INCS
            ${IMGUI_DIR}/backends
    )
endif()

if(IMGUI_ENABLE_SDLRENDERER3)
    list(APPEND IMGUI_IMPL_SRCS
            ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    )
    list(APPEND IMGUI_IMPL_INCS
            ${IMGUI_DIR}/backends
    )
endif()

if(IMGUI_ENABLE_OPENGL3)
    list(APPEND IMGUI_IMPL_SRCS
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
    list(APPEND IMGUI_IMPL_INCS
            ${IMGUI_DIR}/backends
    )
endif()

# ...可以继续加其它实现

add_library(imgui STATIC
        ${IMGUI_CORE_SRCS}
        ${IMGUI_IMPL_SRCS}
)

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_IMPL_INCS}
)
