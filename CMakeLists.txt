cmake_minimum_required(VERSION 3.16)
project(mineclone LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp" "src/*.cpp")

set(IMGUI_DIR vendor/imgui)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(mineclone ${SOURCES} src/glad.c)

target_include_directories(mineclone PRIVATE
        include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)

target_link_libraries(mineclone PRIVATE glfw glm::glm ${CMAKE_DL_LIBS})