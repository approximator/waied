
set(LIB_NAME "waied-submodules")

find_package(Qt5 COMPONENTS Core Gui Widgets Quick QuickWidgets QuickControls2 QmlWorkerScript CONFIG REQUIRED)
message(STATUS "Qt Core: ${Qt5Core_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
file(GLOB_RECURSE SRCS "qt-qml-models/*.cpp")
add_library(${LIB_NAME}
    ${SRCS}
)

target_compile_definitions(${LIB_NAME} PUBLIC "-DQTQMLTRICKS_NO_PREFIX_ON_GETTERS=1" )
target_compile_definitions(${LIB_NAME} PUBLIC "-DQT_STATICPLUGIN" )
target_include_directories(${LIB_NAME} SYSTEM PUBLIC
    qt-super-macros
    qt-qml-models
)

set(LIBS
    qtquick2plugin
    qtquickcontrols2plugin
    qtquicktemplates2plugin
    qtquickcontrols2materialstyleplugin
    qtquickcontrols2universalstyleplugin
    qtquickcontrols2imaginestyleplugin
    qtquickcontrols2fusionstyleplugin
    windowplugin
    qmlsettingsplugin
    modelsplugin
    qquicklayoutsplugin
)

set(FOUND_LIBS "")
file(GLOB_RECURSE QML_DIRS LIST_DIRECTORIES true ${Qt5Core_DIR}/../../../qml *.a)
foreach(QT_LIB ${LIBS})
    find_library(FOUND_LIB_${QT_LIB} ${QT_LIB} PATHS ${QML_DIRS} NO_DEFAULT_PATH)
    message(STATUS ${FOUND_LIB_${QT_LIB}})
    list(APPEND FOUND_LIBS ${FOUND_LIB_${QT_LIB}})
endforeach()

target_link_libraries(${LIB_NAME}
    PUBLIC Qt5::Core
    INTERFACE Qt5::Gui
    ${FOUND_LIBS}
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::QuickControls2
    Qt5::QmlWorkerScript
    CONAN_PKG::date
    CONAN_PKG::fmt
)
