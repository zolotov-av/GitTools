cmake_minimum_required(VERSION 3.16)

project(GitTools VERSION 1.0 LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_custom_target(GitToolsInfo SOURCES
    README.md
)

add_subdirectory(gtcore)
add_subdirectory(GitLog)
