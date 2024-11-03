cmake_minimum_required(VERSION 3.16)

project(GitTools VERSION 1.0 LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Doxygen REQUIRED)

add_custom_target(GitToolsInfo SOURCES
    README.md
    mainpage.dox
)

add_subdirectory(gtcore)
add_subdirectory(GitLog)

doxygen_add_docs(docs
    ${CMAKE_CURRENT_SOURCE_DIR}
    #ALL
    COMMENT "Generating docs"
)
