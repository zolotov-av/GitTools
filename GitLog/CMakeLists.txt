cmake_minimum_required(VERSION 3.16)

project(GitLog VERSION 1.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS DBus)

add_executable(GitLog
    DBusInterface.h
    DBusInterface.cpp
    LogWindow.ui
    LogWindow.h
    LogWindow.cpp
    main.cpp
    ../resources.qrc
    )

set_target_properties(GitLog PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(GitLog PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(GitLog PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(GitLog PROPERTIES AUTOMOC ON)
set_target_properties(GitLog PROPERTIES AUTORCC ON)
set_target_properties(GitLog PROPERTIES AUTOUIC ON)

target_link_libraries(GitLog PRIVATE gtcore Qt5::DBus)
