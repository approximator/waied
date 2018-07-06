
set(LIB_NAME "waied-submodules")

add_subdirectory(fmt)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

add_library(${LIB_NAME}
    ${SRCS}
    ${INCS}
)

target_include_directories(${LIB_NAME} PUBLIC
    qt-super-macros
    qt-qml-models
)

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::QuickControls2
    fmt-header-only
)
