
set(LIB_NAME "waied-submodules")
set(ENABLE_DATE_TESTING OFF CACHE BOOL "Enable unit tests" FORCE)
set(USE_SYSTEM_TZ_DB ON CACHE BOOL "Use the operating system's timezone database" FORCE)
add_subdirectory(date)
add_subdirectory(fmt)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
file(GLOB_RECURSE SRCS "qt-qml-models/*.cpp")
add_library(${LIB_NAME}
    ${SRCS}
)

target_compile_definitions(${LIB_NAME} PUBLIC "-DQTQMLTRICKS_NO_PREFIX_ON_GETTERS=1" )
target_include_directories(${LIB_NAME} PUBLIC
    qt-super-macros
    qt-qml-models
)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)
target_link_libraries(${LIB_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::QuickControls2
    fmt-header-only
    tz
)
