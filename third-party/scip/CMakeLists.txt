if (FVS_USE_SYSTEM_SCIP)
    find_package(scip)
else()
    include(ExternalProject)

    if (UNIX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wno-dev")
    else()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
    endif()

    ExternalProject_Add(scip-project
        GIT_REPOSITORY https://github.com/scipopt/scip
        GIT_TAG v800
        GIT_SHALLOW TRUE
        GIT_CONFIG "advice.detachedHead=false"
        GIT_PROGRESS FALSE
        SOURCE_DIR "${CMAKE_BINARY_DIR}/scip-src"
        BINARY_DIR "${CMAKE_BINARY_DIR}/scip-build"
        CMAKE_ARGS 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
            -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
            -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
            # -DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}
            -DGMP=false
            -DIPOPT=false
            -DPAPILO=false
            -DREADLINE=false
            -DSHARED=true # this also indicates to build static only
            -DSOPLEX_DIR=${CMAKE_BINARY_DIR}/soplex-build
            -DTPI=none
            -DZIMPL=false
            -DZLIB=false
            -DBUILD_TESTING=OFF
            # -DMT=ON
        INSTALL_COMMAND ""
        COMMENT ""
        DOWNLOAD_NO_PROGRESS TRUE
        # BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/scip-build/lib/libscip.a"
    )

    set(SCIP_INCLUDE_DIR ${CMAKE_BINARY_DIR}/scip-src/src;${CMAKE_BINARY_DIR}/scip-build PARENT_SCOPE)
    set(SCIP_LIBRARIES ${CMAKE_BINARY_DIR}/scip-build/lib PARENT_SCOPE)

    add_dependencies(scip-project soplex-project)
endif()