cmake_minimum_required(VERSION 3.10)
project(Flappybird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib CONFIG REQUIRED)

add_executable(Flappybird
  src/main.cpp
)

target_link_libraries(Flappybird PRIVATE raylib)

target_compile_options(Flappybird PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
)

set_target_properties(Flappybird PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

install(TARGETS Flappybird DESTINATION bin)
