cmake_minimum_required(VERSION 3.1)

project(waied LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRCS "*.cpp")
file(GLOB HDRS "*.h")
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} "qml.qrc")

add_subdirectory(submodules/fmt)
find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE submodules/qt-super-macros)
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::QuickControls2
    fmt-header-only
)

# Dependencies
# sudo apt install qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev qtquickcontrols2-5-dev  \
#                  qml-module-qtquick2 qml-module-qtquick-controls libssl-dev



