cmake_minimum_required(VERSION 3.22)

project(shader-converter)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
string(REGEX REPLACE "([\\/\\-]O)3" "\\1z" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

option(ENABLE_ENVIRONMENT_NODE "build for node" OFF)

add_subdirectory(dependencies)

add_executable(shader_converter ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(shader_converter PRIVATE tint_api glslang)

target_link_options(shader_converter
    PRIVATE
    -sMODULARIZE=1
    --js-library=${CMAKE_CURRENT_SOURCE_DIR}/src/callbacks.js
    --extern-pre-js=${CMAKE_CURRENT_SOURCE_DIR}/src/pre.js
    -sEXPORTED_FUNCTIONS=["_free","_malloc","_glsl_to_wgsl"]
    -sEXPORTED_RUNTIME_METHODS=["HEAPU8","lengthBytesUTF8"]
    $<IF:$<BOOL:${ENABLE_ENVIRONMENT_NODE}>,
    -sENVIRONMENT=node -sBINARYEN_ASYNC_COMPILATION=0 --extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/src/post.node.js,
    -sENVIRONMENT=["web"$<COMMA>"worker"] --extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/src/post.web.js
    >
    PUBLIC
    -sSTACK_SIZE=1MB
    -sALLOW_MEMORY_GROWTH=1
)

set_target_properties(shader_converter PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
)
