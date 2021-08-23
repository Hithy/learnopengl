cmake_minimum_required(VERSION 3.10)
project(hello_gl)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 11)

find_package(glfw3 REQUIRED)

add_subdirectory("3rd/glm")

add_executable(hello_gl "src/main.cpp" "src/shader.cpp" "src/camera.cpp" "3rd/src/glad.c")

target_include_directories(hello_gl PUBLIC "3rd/include" "3rd/glm/glm")
target_link_directories(hello_gl PUBLIC "3rd/lib")
target_link_libraries(hello_gl dl glfw glm)
