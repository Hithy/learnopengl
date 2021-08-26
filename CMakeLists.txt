cmake_minimum_required(VERSION 3.10)
project(hello_gl)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 11)

add_executable(hello_gl "src/main.cpp" "src/shader.cpp" "src/camera.cpp" "3rd/src/glad.c")

if (UNIX)
  # linux use system lib
  find_package(glfw3 REQUIRED)
  find_package(glm REQUIRED)
endif(UNIX)

if (WIN32)
  # windows compile from source
  if (NOT EXISTS "3rd/glm")
    execute_process(COMMAND git clone https://github.com/g-truc/glm.git WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
  endif()
  if (NOT EXISTS "3rd/glfw")
    execute_process(COMMAND git clone https://github.com/glfw/glfw.git WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
  endif()

  add_subdirectory("3rd/glm")
  add_subdirectory("3rd/glfw")

  target_include_directories(hello_gl PUBLIC "3rd/glm")
endif(WIN32)

target_include_directories(hello_gl PUBLIC "3rd/include")
target_link_directories(hello_gl PUBLIC "3rd/lib")
if (UNIX)
  target_link_libraries(hello_gl dl)
endif(UNIX)
target_link_libraries(hello_gl glfw)
