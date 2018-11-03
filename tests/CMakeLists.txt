
set(RC_ENABLE_CATCH ON CACHE BOOL "Build Catch.hpp support" FORCE)
add_subdirectory(rapidcheck EXCLUDE_FROM_ALL)

enable_testing()
add_executable(${PROJECT_NAME}_cache_test
    main.cpp
    CacheTest.cpp
    CacheRCTest.cpp
    helpers.h
)

target_link_libraries(${PROJECT_NAME}_cache_test
    PRIVATE
        ${PROJECT_NAME}_lib
        Catch2
        rapidcheck
        rapidcheck_catch
)

add_test(${PROJECT_NAME}_cache_test ${PROJECT_NAME}_cache_test)
