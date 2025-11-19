add_executable(elastic-collisions)

target_sources(elastic-collisions PRIVATE main.cpp world.cpp draw.cpp)
target_compile_features(elastic-collisions PRIVATE cxx_std_23)
set_target_properties(elastic-collisions PROPERTIES CXX_EXTENSIONS OFF)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
    (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    # -Wno-missing-field-initializers due to warnings when compiling inline functions from raymath.h
    target_compile_options(elastic-collisions PRIVATE -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Werror)
endif()

target_link_libraries(elastic-collisions PRIVATE raylib)

if (${PLATFORM} STREQUAL "Web")
    add_custom_command(
        TARGET elastic-collisions PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/web/"
        "$<TARGET_FILE_DIR:elastic-collisions>"
    )

    set_target_properties(elastic-collisions PROPERTIES SUFFIX ".js")
    target_link_options(elastic-collisions PRIVATE --pre-js pre.js)
endif()