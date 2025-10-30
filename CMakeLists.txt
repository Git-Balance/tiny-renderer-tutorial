cmake_minimum_required(VERSION 3.23)

project(tiny-renderer-project)

add_executable(tiny-renderer)

target_sources(tiny-renderer
    PRIVATE
    src/main.cpp
    src/geometry.h
    src/model.cpp
    src/tgaimage.cpp
)
