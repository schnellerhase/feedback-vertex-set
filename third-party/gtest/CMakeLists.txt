
cmake_minimum_required(VERSION 3.5.0)

option(USE_SYSTEM_GOOGLETEST "Use system googletest and do not download/build an own version of it" ON)

if(USE_SYSTEM_GOOGLETEST)

  if("${CMAKE_VERSION}" VERSION_LESS "3.11")
    function(add_library)
      set(_args ${ARGN})
      if ("${_args}" MATCHES ";IMPORTED")
        list(APPEND _args GLOBAL)
      endif()
      _add_library(${_args})
    endfunction()
  endif()

  find_package(GTest REQUIRED)

  if("${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.11")
    set_target_properties(GTest::GTest PROPERTIES
      IMPORTED_GLOBAL TRUE
    )
    set_target_properties(GTest::Main PROPERTIES
      IMPORTED_GLOBAL TRUE
    )
  endif()

  add_library(third_party::gtest INTERFACE IMPORTED GLOBAL)
  add_library(third_party::gtest_main INTERFACE IMPORTED GLOBAL)

  set_target_properties(third_party::gtest PROPERTIES
    INTERFACE_LINK_LIBRARIES GTest::GTest
  )
  set_target_properties(third_party::gtest_main PROPERTIES
    INTERFACE_LINK_LIBRARIES GTest::Main
  )
else()
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()
