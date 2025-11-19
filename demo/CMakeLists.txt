add_executable(demo)

target_sources(demo PRIVATE main.cpp)
target_compile_features(demo PRIVATE cxx_std_23)
set_target_properties(demo PROPERTIES CXX_EXTENSIONS OFF)

if(
    (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
    (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    # -Wno-missing-field-initializers due to warnings when compiling inline functions from raymath.h
    target_compile_options(demo PRIVATE -Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Werror)
endif()

target_link_libraries(demo PRIVATE raylib)

if (${PLATFORM} STREQUAL "Web")
    add_custom_command(
        TARGET demo PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/web/"
        "$<TARGET_FILE_DIR:demo>"
    )

    set_target_properties(demo PROPERTIES SUFFIX ".js")
    target_link_options(demo PRIVATE --pre-js pre.js)
endif()