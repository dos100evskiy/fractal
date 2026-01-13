cmake_minimum_required(VERSION 3.10)
project(FractalsQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED) # Или Qt6, если используешь

add_executable(fractals main.cpp)
target_link_libraries(fractals Qt5::Core Qt5::Gui Qt5::Widgets) # Или Qt6::
