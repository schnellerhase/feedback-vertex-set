
function(build_benchmark FILE)
    get_filename_component(NAME ${FILE} NAME_WE)
    add_executable(benchmark_${NAME} ${FILE})
    add_dependencies(benchmark_${NAME} google-benchmark)
    target_include_directories(benchmark_${NAME} PUBLIC ${CMAKE_BINARY_DIR}/google-benchmark-src/include)
    target_include_directories(benchmark_${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-src/src)
    target_include_directories(benchmark_${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-build)
    target_include_directories(benchmark_${NAME} SYSTEM PUBLIC ../../third-party/)
    target_include_directories(benchmark_${NAME} PUBLIC ../../src)
    target_include_directories(benchmark_${NAME} PUBLIC ../../fvs_solver/src)

    target_include_directories(benchmark_${NAME} PUBLIC ../src)

    target_link_directories(benchmark_${NAME} PUBLIC ${CMAKE_BINARY_DIR}/scip-build/lib)
    target_link_directories(benchmark_${NAME} PUBLIC ${CMAKE_BINARY_DIR}/soplex-build/lib)

    add_dependencies(benchmark_${NAME} scip soplex tracks)

    target_link_libraries(benchmark_${NAME} ${CMAKE_BINARY_DIR}/google-benchmark-build/src/Release/libbenchmark.a pthread scip.so soplex.so)
endfunction()

file(GLOB_RECURSE CPPS "*.cpp")
foreach(CPP ${CPPS})
    build_benchmark(${CPP})
endforeach()