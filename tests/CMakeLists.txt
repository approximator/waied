
add_subdirectory(catch)
add_subdirectory(rapidcheck)

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
