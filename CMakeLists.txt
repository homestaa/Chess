cmake_minimum_required(VERSION 3.10)

project(Chess)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# required environment variable "SDL2DIR" set to local SDL2- and SDL2_image-folder
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/sdl2")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})


set(copy_source_dir "${CMAKE_SOURCE_DIR}")
set(copy_dest_dir "${CMAKE_BINARY_DIR}")
set(copy_file_name "res")

# Add sub-projects for editor and game
add_subdirectory(src)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


