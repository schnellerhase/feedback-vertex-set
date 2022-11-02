include(ExternalProject)

if (UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wno-dev")
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
endif()

ExternalProject_Add(scip-shared
    GIT_REPOSITORY https://github.com/scipopt/scip
    GIT_TAG v800
    GIT_SHALLOW TRUE
    GIT_CONFIG "advice.detachedHead=false"
    GIT_PROGRESS FALSE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/scip-shared-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/scip-shared-build"
    CMAKE_ARGS 
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
        -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
        -DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}
        -DGMP=false
        -DIPOPT=false
        -DPAPILO=false
        -DREADLINE=false
        -DSOPLEX_DIR=${CMAKE_BINARY_DIR}/soplex-build
        -DTPI=none
        -DZIMPL=false
        -DZLIB=false
        -DSHARED=true
        -DBUILD_TESTING=OFF
    INSTALL_COMMAND ""
    COMMENT ""
    DOWNLOAD_NO_PROGRESS TRUE
)

add_dependencies(scip soplex)
