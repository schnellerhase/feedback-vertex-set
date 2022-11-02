link_libraries(GTest::Main)

file(COPY data DESTINATION .)

file(GLOB_RECURSE CPPS "*.cpp")
foreach(CPP ${CPPS})
    get_filename_component(TEST_NAME ${CPP} NAME_WE)
    add_executable(test_${TEST_NAME} ${CPP})

    target_include_directories(test_${TEST_NAME} PUBLIC ../src)
    target_include_directories(test_${TEST_NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-src/src)
    target_include_directories(test_${TEST_NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-build)
    target_include_directories(test_${TEST_NAME} SYSTEM PUBLIC ../third-party/)
    
    target_link_directories(test_${TEST_NAME} PUBLIC ${CMAKE_BINARY_DIR}/scip-build/lib)
    target_link_directories(test_${TEST_NAME} PUBLIC ${CMAKE_BINARY_DIR}/soplex-build/lib)
    target_link_libraries(test_${TEST_NAME} scip.so soplex.so)

    add_dependencies(test_${TEST_NAME} scip soplex)

    add_test(NAME test_${TEST_NAME} COMMAND test_${TEST_NAME})
endforeach()
