cmake_minimum_required(VERSION 3.14)
project(comp_geoT1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# ----------------------------------------------------------------------------
# Core: Punto / Vector / Poligono. The assignment mandates templates, so every
# body lives in the header -- there is nothing to compile, hence INTERFACE.
# Anything that links `geo` gets include/ on its include path.
# ----------------------------------------------------------------------------
add_library(geo INTERFACE)
target_include_directories(geo INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(geo INTERFACE cxx_std_17)

# ----------------------------------------------------------------------------
# Demo application. Gated on the source existing so the tree still configures
# while src/ is empty.
# ----------------------------------------------------------------------------
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
  add_executable(main src/main.cpp)
  target_link_libraries(main PRIVATE geo)
endif()

# ----------------------------------------------------------------------------
# Tests. Prefer the system GTest (Arch's `gtest` package ships the CMake config
# in /usr/lib/cmake/GTest); fall back to building it from upstream when absent.
# ----------------------------------------------------------------------------
file(GLOB GEO_TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)

if(GEO_TEST_SOURCES)
  find_package(GTest CONFIG QUIET)

  if(NOT GTest_FOUND)
    message(STATUS "GTest not found locally, fetching from GitHub...")
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.17.0
        )
    FetchContent_MakeAvailable(googletest)
  endif()

  include(GoogleTest)

  # One binary for the whole suite; gtest_main supplies main(). CONFIGURE_DEPENDS
  # above re-globs on build, so a new test/*.cpp is picked up without editing this.
  add_executable(geo_tests ${GEO_TEST_SOURCES})
  target_link_libraries(geo_tests PRIVATE geo GTest::gtest_main)
  gtest_discover_tests(geo_tests)
endif()
