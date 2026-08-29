cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

project(ASCIIOR LANGUAGES C)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/console.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
