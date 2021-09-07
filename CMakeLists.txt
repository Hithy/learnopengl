cmake_minimum_required(VERSION 3.10)
project(hello_gl)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SRC_FILES src/*.cpp)
add_executable(hello_gl
  ${SRC_FILES}
  "3rd/src/glad.c")

function(fetch_git_repo path name)
  if (NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/${name}")
    execute_process(COMMAND git clone ${path} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
  endif()
  add_subdirectory("3rd/${name}")
endfunction()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # msvc compile from source
  fetch_git_repo("https://github.com/g-truc/glm.git" "glm")
  fetch_git_repo("https://github.com/glfw/glfw.git" "glfw")
  fetch_git_repo("https://github.com/assimp/assimp.git" "assimp")

  target_include_directories(hello_gl PUBLIC "3rd/glm")
  target_include_directories(hello_gl PUBLIC "3rd/assimp/include")
else()
  find_package(glfw3 REQUIRED)
  find_package(glm REQUIRED)
  find_package(assimp REQUIRED)
endif()

target_include_directories(hello_gl PUBLIC "3rd/include")
target_link_directories(hello_gl PUBLIC "3rd/lib")
if (UNIX)
  target_link_libraries(hello_gl dl)
endif(UNIX)
target_link_libraries(hello_gl glfw assimp)

add_custom_target(run
  COMMAND hello_gl
  DEPENDS hello_gl
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  )
