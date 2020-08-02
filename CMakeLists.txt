cmake_minimum_required(VERSION 3.10)
project(screen_snipe)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(X11 REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 14)

file(GLOB IMAGE_RESOURCES "resources/*.png")
file(COPY ${IMAGE_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

set(qhotkey_src QHotkey/QHotkey/qhotkey.cpp QHotkey/QHotkey/qhotkey_x11.cpp)

add_executable(screen_snipe ${qhotkey_src} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC QHotkey/QHotkey)
target_link_libraries(screen_snipe Qt5::Widgets Qt5::X11Extras X11)