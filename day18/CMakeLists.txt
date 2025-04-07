cmake_minimum_required(VERSION 3.30)

project(day18)

set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(PROJECT_DAY="18")

add_executable(${PROJECT_NAME}
        ../main.cpp
        ${PROJECT_NAME}.cpp
)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/input.txt)
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/input.txt"
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/input.txt"
            COMMENT "Copying input.txt to output directory"
    )
endif ()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.txt)
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/test.txt"
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test.txt ${CMAKE_CURRENT_BINARY_DIR}/test.txt
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test.txt"
            COMMENT "Copying test.txt to output directory"
    )
endif ()

add_custom_target(${PROJECT_NAME}_copyfiles ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/input.txt ${CMAKE_CURRENT_BINARY_DIR}/test.txt)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_copyfiles)

if (CMAKE_BUILD_TYPE=="DEBUG")
    find_package(Catch2 CONFIG REQUIRED)
    add_executable(${PROJECT_NAME}_tests
            ${PROJECT_NAME}.cpp)
    target_compile_definitions(${PROJECT_NAME}_tests PUBLIC CATCH2)
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)


    add_dependencies(${PROJECT_NAME}_tests ${PROJECT_NAME}_copyfiles)
endif ()