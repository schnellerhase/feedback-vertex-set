cmake_minimum_required(VERSION 3.11)

project(FVS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_BUILD_TYPE "" RELEASE)
option(BUILD_TESTING "Compile tests" OFF)
option(BUILD_BENCHMARKS "Compile benchmarks" OFF)
option(ENABLE_PROFILING "Profiling support" OFF)

option(FVS_USE_SYSTEM_SOPLEX ON)
option(FVS_USE_SYSTEM_SCIP ON)

include(CTest)

if (CMAKE_BUILD_TYPE MATCHES RELEASE)
    if (UNIX)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") # TODO: experiment with -Ofast
    else()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /02")
    endif()
endif()

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    if (UNIX)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    else()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Z7")
    endif()
endif()

if (PROFILING)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

add_subdirectory(third-party/scip)
add_subdirectory(third-party/soplex)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror") 
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX ") # Wall causes some weird warnings
endif()

# download graphs -> move to some other place?
if (BUILD_TESTING OR BUILD_BENCHMARKS)
    include(ExternalProject)
    ExternalProject_Add(
        tracks 
        SOURCE_DIR "${CMAKE_BINARY_DIR}/tracks"
        URL https://heibox.uni-heidelberg.de/f/be4337d9e4234bca8606/?dl=1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(SYSTEM PUBLIC ${SCIP_INCLUDE_DIR})
include_directories(SYSTEM PUBLIC ${SOPLEX_INCLUDE_DIR})
include_directories(SYSTEM PUBLIC third-party)

link_directories(PUBLIC ${SOPLEX_LIBRARIES})
link_directories(PUBLIC ${SCIP_LIBRARIES})

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()

function(SOLVER NAME)
    add_executable(${NAME} solvers/${NAME}.cpp)
    target_include_directories(${NAME} SYSTEM PUBLIC third-party)

    # STATIC BUILD
    # target_link_libraries(${NAME} -static)
    # target_link_libraries(${NAME} scip_static_lib soplex_static_lib)
    # add_dependencies(${NAME} scip soplex)

    if (UNIX)
        target_link_libraries(${NAME} scip soplex)
    else()
        target_link_libraries(${NAME} libscip libsoplex)
    endif()

    if (NOT FVS_USE_SYSTEM_SCIP)
        add_dependencies(${NAME} scip-project)
    endif()

    if (NOT FVS_USE_SYSTEM_SOPLEX)
        add_dependencies(${NAME} soplex-project)
    endif()
endfunction()

SOLVER(naive)
SOLVER(reduce)
SOLVER(greedy_reduction)
SOLVER(vertex_cover)
SOLVER(vc_reduce)
SOLVER(reduce_deg_1)
SOLVER(reduce_deg_1_scc)

if (BUILD_BENCHMARKS)
    find_package(benchmark REQUIRED)

    function(build_benchmark FILE)
        get_filename_component(NAME ${FILE} NAME_WE)
        add_executable(benchmark_${NAME} ${FILE})

        target_include_directories(benchmark_${NAME} PUBLIC ${CMAKE_SOURCE_DIR})
        # target_include_directories(benchmark_${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-src/src)
        # target_include_directories(benchmark_${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-build)
        # target_include_directories(benchmark_${NAME} SYSTEM PUBLIC third-party/)

        # target_link_directories(benchmark_${NAME} PUBLIC ${CMAKE_BINARY_DIR}/scip-build/lib)
        # target_link_directories(benchmark_${NAME} PUBLIC ${CMAKE_BINARY_DIR}/soplex-build/lib)

        add_dependencies(benchmark_${NAME} tracks)
        if (NOT FVS_USE_SYSTEM_SCIP)
            add_dependencies(benchmark_${NAME} scip-project)
        endif()

        if (NOT FVS_USE_SYSTEM_SOPLEX)
            add_dependencies(benchmark_${NAME} soplex-project)
        endif()

        target_link_libraries(benchmark_${NAME} benchmark::benchmark pthread scip soplex)
    endfunction()

    file(GLOB_RECURSE CPPS "benchmark/*.cpp")
    foreach(CPP ${CPPS})
        build_benchmark(${CPP})
    endforeach()
endif()
