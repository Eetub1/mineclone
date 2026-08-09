cmake_minimum_required(VERSION 3.16)
project(mineclone LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB SOURCES "*.cpp" "src/*.cpp")
add_executable(mineclone ${SOURCES} src/glad.c)

target_include_directories(mineclone PRIVATE include)

target_link_libraries(mineclone PRIVATE glfw glm::glm ${CMAKE_DL_LIBS})