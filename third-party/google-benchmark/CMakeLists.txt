include(ExternalProject)

ExternalProject_Add(google-benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.6.1
    GIT_SHALLOW TRUE
    GIT_CONFIG "advice.detachedHead=false"
    GIT_PROGRESS FALSE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/google-benchmark-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/google-benchmark-build"
    CMAKE_ARGS
        -DBENCHMARK_ENABLE_TESTING=OFF
        -DCMAKE_BUILD_TYPE=Release
        # -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=Debug
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=Release
        # -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO:PATH=RelWithDebInfo
    INSTALL_COMMAND ""
    LOG_DOWNLOAD OFF
    LOG_CONFIGURE OFF
    LOG_BUILD OFF
    COMMENT ""
    BUILD_BYPRODUCTS "${CMAKE_BINARY_DIR}/google-benchmark-build/src/Release/libbenchmark.a"
)
