
add_subdirectory(catch EXCLUDE_FROM_ALL)
add_subdirectory(rapidcheck EXCLUDE_FROM_ALL)

enable_testing()
add_executable(${PROJECT_NAME}_test
    main.cpp
)

target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
        ${PROJECT_NAME}_lib
        Catch2
        rapidcheck
)

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
