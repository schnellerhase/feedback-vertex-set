include(ExternalProject)

ExternalProject_Add(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.10.5
    GIT_SHALLOW TRUE
    GIT_CONFIG "advice.detachedHead=false"
    GIT_PROGRESS FALSE
    SOURCE_DIR "${CMAKE_BINARY_DIR}/json-src"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    COMMENT ""
    DOWNLOAD_NO_PROGRESS TRUE
)
