add_executable(analysis graph-properties.cpp)

add_dependencies(analysis ogdf)
add_dependencies(analysis json)

target_include_directories(analysis SYSTEM PUBLIC ../../src)
target_include_directories(analysis SYSTEM PRIVATE ${CMAKE_BINARY_DIR}/ogdf-src/include)
target_include_directories(analysis SYSTEM PRIVATE ${CMAKE_BINARY_DIR}/ogdf-build/include)
target_include_directories(analysis SYSTEM PRIVATE ${CMAKE_BINARY_DIR}/json-src/include)

target_link_directories(analysis PRIVATE ${CMAKE_BINARY_DIR}/ogdf-build)

target_link_libraries(analysis libOGDF.a libCOIN.a pthread)
