set(CODE_SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/boundary.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/grid.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/properties.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/scheme.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/solver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/temperature.cpp
)

add_library(_logic
        SHARED 
                ${CODE_SOURCE})

target_include_directories(_logic 
    PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(_logic PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)
