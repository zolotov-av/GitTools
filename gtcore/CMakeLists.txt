cmake_minimum_required(VERSION 3.16)

project(GtCore VERSION 1.0 LANGUAGES C CXX)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Quick)

add_library(gtcore STATIC
    GitTools/CommitInfo.h
    GitTools/CreateBranchDialog.h
    GitTools/DeleteBranchDialog.h
    GitTools/ExceptionTooltip.h
    GitTools/GitCommitFiles.h
    GitTools/GitLogDelegate.h
    GitTools/GitLogModel.h
    GitTools/base.h
    GitTools/string.h
    GitTools/exception.h
    GitTools/GitCommitFiles.cpp
    GitTools/GitLogDelegate.cpp
    GitTools/GitLogModel.cpp
    GitTools/CreateBranchDialog.ui
    GitTools/DeleteBranchDialog.ui
    GitTools/ExceptionTooltip.ui
    GitTools/GraphItem.h
    GitTools/GraphItem.cpp
    GitTools/DiffModel.h
    GitTools/DiffModel.cpp
)

set_target_properties(gtcore PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(gtcore PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(gtcore PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(gtcore PROPERTIES AUTOMOC ON)
set_target_properties(gtcore PROPERTIES AUTORCC ON)
set_target_properties(gtcore PROPERTIES AUTOUIC ON)

target_include_directories(gtcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(gtcore PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/gtcore_autogen/include)

target_link_libraries(gtcore PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Quick)
target_link_libraries(gtcore PUBLIC -lgit2)
