find_package(GTest REQUIRED)

set_target_properties(GTest::GTest PROPERTIES IMPORTED_GLOBAL TRUE)
set_target_properties(GTest::Main PROPERTIES IMPORTED_GLOBAL TRUE)

link_libraries(GTest::Main)

file(COPY data DESTINATION .)

file(GLOB_RECURSE CPPS "*.cpp")
foreach(CPP ${CPPS})
    get_filename_component(TEST_NAME ${CPP} NAME_WE)
    add_executable(test_${TEST_NAME} ${CPP})

    target_include_directories(test_${TEST_NAME} PUBLIC ../)
    target_link_libraries(test_${TEST_NAME} scip soplex)

    if (NOT FVS_USE_SYSTEM_SCIP)
        add_dependencies(test_${TEST_NAME} scip-project)
    endif()

    if (NOT FVS_USE_SYSTEM_SOPLEX)
        add_dependencies(test_${TEST_NAME} soplex-project)
    endif()
    add_test(NAME test_${TEST_NAME} COMMAND test_${TEST_NAME})
endforeach()
