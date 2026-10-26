# Fetch GoogleTest directly from the official repository
include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY
	https://github.com/google/googletest.git
	GIT_TAG
	v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(engine_tests
	test_sanity.cpp
	test_card.cpp
	test_deck.cpp
	test_pile.cpp
	../src/core/Card.cpp
	../src/core/Deck.cpp
	../src/core/Pile.cpp
)

# Tell the test executable where to find the header files
target_include_directories(engine_tests PRIVATE ../src)

# Link GoogleTest to our test executable
target_link_libraries(engine_tests GTest::gtest_main)

# Register tests with CTest (CMake's test runner)
include(GoogleTest)
gtest_discover_tests(engine_tests)
