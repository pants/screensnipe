cmake_minimum_required(VERSION 3.10)
project(screen_snipe)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(X11 REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 14)

set(qhotkey_src QHotkey/QHotkey/qhotkey.cpp QHotkey/QHotkey/qhotkey_x11.cpp)

add_executable(screen_snipe ${qhotkey_src} src/main.cpp src/window/capture_window.cpp src/window/capture_window.h src/main/screen_capturer.cpp src/main/screen_capturer.h src/window/settings_window.cpp src/window/settings_window.h src/main/screen_snipe.cpp src/main/screen_snipe.h)

target_include_directories(${PROJECT_NAME} PUBLIC QHotkey/QHotkey)
target_link_libraries(screen_snipe Qt5::Widgets Qt5::X11Extras X11)