# Fetch GoogleTest directly from the official repository
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(engine_tests test_sanity.cpp)

# Link GoogleTest to our test executable
target_link_libraries(engine_tests GTest::gtest_main)

# Register tests with CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(engine_tests)
