cmake_minimum_required(VERSION 3.10...3.28)

project(bar_model)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(code)

add_executable(bar_model 
    ${CMAKE_CURRENT_SOURCE_DIR}/code/src/main.cpp)

target_link_libraries(bar_model PRIVATE _logic)

set_target_properties(bar_model PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin" 
)	
