cmake_minimum_required(VERSION 3.16)
project(mineclone LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(mineclone
        main.cpp
        src/glad.c
)

target_include_directories(mineclone PRIVATE include)

target_link_libraries(mineclone PRIVATE glfw glm::glm ${CMAKE_DL_LIBS})