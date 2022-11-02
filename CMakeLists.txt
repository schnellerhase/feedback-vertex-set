cmake_minimum_required(VERSION 3.11)

project(PACE_2022)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

option(BUILD_TESTING "Compile tests" OFF)
option(ENABLE_PROFILING "Profiling support" OFF)
option(ENABLE_BENCHMARK "Compile benchmarks" OFF)

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

add_subdirectory(third-party)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -w") 
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /WX")
endif()

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
# file(GLOB_RECURSE DISCRETE_SRCS "src/discrete/*.cpp")
# add_library(discrete_LIB STATIC ${DISCRETE_SRCS})

function(SOLVER NAME)
    add_executable(${NAME} src/solvers/${NAME}.cpp)

    target_include_directories(${NAME} PUBLIC src)
    target_include_directories(${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-src/src)
    target_include_directories(${NAME} SYSTEM PUBLIC ${CMAKE_BINARY_DIR}/scip-build)
    target_include_directories(${NAME} SYSTEM PUBLIC third-party)

    target_link_directories(${NAME} PUBLIC ${CMAKE_BINARY_DIR}/scip-build/lib)
    target_link_directories(${NAME} PUBLIC ${CMAKE_BINARY_DIR}/soplex-build/lib)

    # UNCOMMENT THIS FOR STATIC BUILD
    # target_link_libraries(${NAME} -static)
    # target_link_libraries(${NAME} scip_static_lib soplex_static_lib)
    # add_dependencies(${NAME} scip soplex)

    # UNCOMMEN THIS FOR SHARED BUILD
    target_link_libraries(${NAME} scip.so soplex.so)
    add_dependencies(${NAME} scip soplex)
endfunction()

SOLVER(naive)
SOLVER(reduce)
SOLVER(greedy_reduction)
SOLVER(vertex_cover)
SOLVER(vc_reduce)
SOLVER(reduce_deg_1)
SOLVER(reduce_deg_1_scc)

if (ENABLE_BENCHMARK)
    add_subdirectory(benchmark/solvers)
endif()
# add_subdirectory(tools) # flag this -> we do not want to build this by default. (OGDF only required if tools...)

# download graphs -> move to some ohter place?
include(ExternalProject)
ExternalProject_Add(
    tracks 
    SOURCE_DIR "${CMAKE_BINARY_DIR}/tracks"
    URL https://heibox.uni-heidelberg.de/f/be4337d9e4234bca8606/?dl=1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)