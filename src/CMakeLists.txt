
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SRCS "*.cpp")
file(GLOB HDRS "*.h")
add_executable(${PROJECT_NAME}
    ${SRCS}
    ${HDRS}
    "ui/qml.qrc"
    "ui/fonts.qrc"
    "settings.qrc"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Qml
    Qt5::QuickControls2
    waied-submodules
)

if (MSVC)
    target_compile_options(${PROJECT_NAME}
        /std:c++latest
        /W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265
        /w44296 /w44311 /w44826 /we4289 /w14546 /w14547
        /w14549 /w14555 /w14619 /w14905 /w14906 /w14928)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
#        -Wconversion
#        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wcast-qual
        -Wunused
        -Woverloaded-virtual
        -Wpedantic
#        -Weffc++
    )
endif()
