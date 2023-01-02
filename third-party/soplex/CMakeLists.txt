if (FVS_USE_SYSTEM_SOPLEX)
        
    find_package(soplex)

else()

    include(ExternalProject)

    if (UNIX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wno-dev")
    else()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
    endif()

    # set (SOPLEX_SOURCE_DIR "" PARENT_SCOPE )
    # set (SOPLEX_BINARY_DIR "" PARENT_SCOPE )

    ExternalProject_Add(soplex-project
        GIT_REPOSITORY https://github.com/scipopt/soplex
        GIT_TAG release-600
        GIT_SHALLOW TRUE
        GIT_CONFIG "advice.detachedHead=false"
        GIT_PROGRESS FALSE
        SOURCE_DIR "${CMAKE_BINARY_DIR}/soplex-src"
        BINARY_DIR "${CMAKE_BINARY_DIR}/soplex-build"
        CMAKE_ARGS
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
            -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
            -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
            -DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}
            -DBUILD_TESTING=OFF
            -DZLIB=FALSE
            -DGMP=FALSE
            -DBOOST=FALSE
            # -DMT=TRUE
        INSTALL_COMMAND ""
        # BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/soplex-build/lib/libsoplex.a"
        COMMENT ""
        DOWNLOAD_NO_PROGRESS TRUE
    )

    set(SOPLEX_INCLUDE_DIR ${CMAKE_BINARY_DIR}/soplex-src/src;${CMAKE_BINARY_DIR}/soplex-build PARENT_SCOPE)
    set(SOPLEX_LIBRARIES ${CMAKE_BINARY_DIR}/soplex-build/lib PARENT_SCOPE)

    # add_library(soplex_static_lib STATIC IMPORTED GLOBAL)
    # set_target_properties(soplex_static_lib PROPERTIES IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/soplex-build/lib/libsoplex.a")
endif()