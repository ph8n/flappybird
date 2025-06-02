cmake_minimum_required(VERSION 3.10)
project(Flappybird LANGUAGES CXX)

add_executable(Flappybird
  src/main.cpp
)

find_package(raylib CONFIG REQUIRED)

target_link_libraries(Flappybird PRIVATE raylib)

target_compile_features(Flappybird PRIVATE cxx_std_20)
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
