find_package(GTest REQUIRED)

set_target_properties(GTest::GTest PROPERTIES
  IMPORTED_GLOBAL TRUE
)
set_target_properties(GTest::Main PROPERTIES
  IMPORTED_GLOBAL TRUE
)

add_library(third_party::gtest INTERFACE IMPORTED GLOBAL)
add_library(third_party::gtest_main INTERFACE IMPORTED GLOBAL)

set_target_properties(third_party::gtest PROPERTIES
  INTERFACE_LINK_LIBRARIES GTest::GTest
)
set_target_properties(third_party::gtest_main PROPERTIES
  INTERFACE_LINK_LIBRARIES GTest::Main
)
