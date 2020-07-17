cmake_minimum_required(VERSION 3.10)
project(screensnipe)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(X11 REQUIRED)
find_package(Qt5X11Extras REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 14)

set(qhotkey_src QHotkey/QHotkey/qhotkey.cpp QHotkey/QHotkey/qhotkey_x11.cpp)

add_executable(screensnipe ${qhotkey_src} src/main.cpp src/window/CaptureWindow.cpp src/window/CaptureWindow.h src/main/ScreenCapturer.cpp src/main/ScreenCapturer.h src/window/SettingsWindow.cpp src/window/SettingsWindow.h src/main/ScreenSnipe.cpp src/main/ScreenSnipe.h)

target_include_directories(${PROJECT_NAME} PUBLIC QHotkey/QHotkey)
target_link_libraries(screensnipe Qt5::Widgets Qt5::X11Extras X11)