cmake_minimum_required(VERSION 3.1)

project(waied LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp" "qml.qrc")

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Qml Qt5::QuickControls2)



# sudo apt install qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev qtquickcontrols2-5-dev qml-module-qtquick2 qml-module-qtquick-controls



