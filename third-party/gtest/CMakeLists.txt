option(FETCH_GOOGLETEST "Enable fetching of google test" OFF)

if (FETCH_GOOGLETEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

find_package(GTest REQUIRED)

set_target_properties(GTest::GTest PROPERTIES
  IMPORTED_GLOBAL TRUE
)
set_target_properties(GTest::Main PROPERTIES
  IMPORTED_GLOBAL TRUE
)

# add_library(third_party::gtest INTERFACE IMPORTED GLOBAL)
# add_library(third_party::gtest_main INTERFACE IMPORTED GLOBAL)

# set_target_properties(third_party::gtest PROPERTIES
#   INTERFACE_LINK_LIBRARIES GTest::GTest
# )
# set_target_properties(third_party::gtest_main PROPERTIES
#   INTERFACE_LINK_LIBRARIES GTest::Main
# )
