cmake_minimum_required(VERSION 3.30)
project(day18)

set(CMAKE_CXX_STANDARD 23)
find_package(Catch2 CONFIG REQUIRED)

add_compile_definitions(PROJECT_DAY="18")

add_executable(${PROJECT_NAME}_tests
        ${PROJECT_NAME}.cpp)
target_compile_definitions(${PROJECT_NAME}_tests PUBLIC CATCH2)
target_link_libraries(${PROJECT_NAME}_tests PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_executable(${PROJECT_NAME}
        ../main.cpp
        ${PROJECT_NAME}.cpp
)

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/input.txt"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt"
        COMMENT "Copying input.txt to output directory"
)

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/test1.txt"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test1.txt ${CMAKE_CURRENT_BINARY_DIR}/test1.txt
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test1.txt"
        COMMENT "Copying test1.txt to output directory"
)

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/test2.txt"
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test2.txt ${CMAKE_CURRENT_BINARY_DIR}/test2.txt
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test2.txt"
        COMMENT "Copying test2.txt to output directory"
)

add_custom_target(${PROJECT_NAME}_copyfiles ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/test1.txt ${CMAKE_CURRENT_BINARY_DIR}/test2.txt)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_copyfiles)
add_dependencies(${PROJECT_NAME}_tests ${PROJECT_NAME}_copyfiles)