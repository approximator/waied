find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME}_lib
    jira.cpp
    jira.h
    cache.cpp
    cache.h
    task.cpp
    task.h
    worklog.cpp
    worklog.h
    taskmanager.cpp
    taskmanager.h
    sortfilterproxymodel.cpp
    sortfilterproxymodel.h
    log.h
)
target_compile_features(${PROJECT_NAME}_lib PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}_lib PUBLIC
    waied-submodules
)

add_executable(${PROJECT_NAME}
    main.cpp
    ui/qml.qrc
    ui/fonts.qrc
    settings.qrc
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_lib
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

if (MSVC)

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
        -Weffc++
    )
endif()
