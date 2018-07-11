
set(LIB_NAME "waied-submodules")

add_subdirectory(fmt)
add_subdirectory(date)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

file(GLOB_RECURSE SRCS "qt-qml-models/*.cpp")

add_library(${LIB_NAME}
    ${SRCS}
)

target_include_directories(${LIB_NAME} PUBLIC
    qt-super-macros
    qt-qml-models
)

target_compile_definitions(${LIB_NAME} PUBLIC "-DQTQMLTRICKS_NO_PREFIX_ON_GETTERS=1" )

target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::QuickControls2
    fmt-header-only
    tz
)
