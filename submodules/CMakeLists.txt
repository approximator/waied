set(LIB_NAME "waied-submodules")

if (USE_CONAN)
    message(STATUS "Using conan for deps")
    # Download automatically, you can also just copy the conan.cmake file
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                        "${CMAKE_BINARY_DIR}/conan.cmake"
                        TLS_VERIFY ON)
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    set(STATIC_OPTION "")
    if (STATIC_BUILD)
        set(STATIC_OPTION "*:shared=False")
    endif()
    conan_cmake_run(REQUIRES qt/5.15.0@bincrafters/stable
                             openssl/1.1.1g
                             date/2.4.1
                             fmt/6.1.2
                             gtest/1.10.0
                             rapidcheck/20200131
                    BASIC_SETUP CMAKE_TARGETS
                    UPDATE BUILD outdated
                    OPTIONS
                        ${STATIC_OPTION}
                        qt:GUI=True
                        qt:qtsvg=True
                        qt:widgets=True
                        qt:qtdeclarative=True
                        qt:qtmultimedia=True
                        qt:qtxmlpatterns=True
                        qt:qttranslations=True
                        qt:qtquickcontrols=True
                        qt:qtquickcontrols2=True
                        qt:qtx11extras=True
                        qt:qtimageformats=True
                        qt:qtgraphicaleffects=True
                        qt:qttools=True
                        qt:qtconnectivity=True
                        qt:qtnetworkauth=True
                        qt:with_freetype=True
                        qt:with_pq=False
                        qt:with_odbc=False
                        qt:with_sqlite3=True
                        qt:with_fontconfig=False
                        glib:shared=True
                    )

    set(DEP_LIBS
        CONAN_PKG::date
        CONAN_PKG::fmt)
else ()
    message(STATUS "Conan is NOT used.")

    set(ENABLE_DATE_TESTING OFF CACHE BOOL "Enable unit tests" FORCE)
    set(USE_SYSTEM_TZ_DB ON CACHE BOOL "Use the operating system's timezone database" FORCE)
    add_subdirectory(date)
    add_subdirectory(fmt)

    set(DEP_LIBS
        fmt-header-only
        date_interface)
endif ()


find_package(Qt5 COMPONENTS Core Gui Widgets Quick QuickWidgets QuickControls2 CONFIG REQUIRED)
message(STATUS "Qt Core: ${Qt5Core_DIR}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SRCS "qt-qml-models/*.cpp")
add_library(${LIB_NAME}
    ${SRCS}
)

target_compile_definitions(${LIB_NAME} PUBLIC "-DQTQMLTRICKS_NO_PREFIX_ON_GETTERS=1" )
target_include_directories(${LIB_NAME} SYSTEM PUBLIC
    qt-super-macros
    qt-qml-models
)

set(STATIC_LIBS "")
if (STATIC_BUILD)
    add_definitions("-DQT_STATICPLUGIN")
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

    file(GLOB_RECURSE QML_DIRS LIST_DIRECTORIES true ${Qt5Core_DIR}/../../../qml *.a)
    foreach(QT_LIB ${LIBS})
        find_library(FOUND_LIB_${QT_LIB} ${QT_LIB} PATHS ${QML_DIRS} NO_DEFAULT_PATH)
        message(STATUS ${FOUND_LIB_${QT_LIB}})
        list(APPEND STATIC_LIBS ${FOUND_LIB_${QT_LIB}})
    endforeach()
endif()

target_link_libraries(${LIB_NAME}
    PUBLIC Qt5::Core
    INTERFACE Qt5::Gui
    ${STATIC_LIBS}
    Qt5::Widgets
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickWidgets
    Qt5::QuickControls2
    ${DEP_LIBS}
)
