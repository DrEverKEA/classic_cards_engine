# ==============================================================================
# Project: classic_cards_engine
# Description: C++14 MVC Engine for Classic Solitaire Games
#
# Reference Documentation for Build System:
# - Official CMake Docs: https://cmake.org/documentation/
# - Modern CMake Guide:  https://cliutils.gitlab.io/modern-cmake/
# ==============================================================================

cmake_minimum_required(VERSION 3.20)
project(classic_cards_engine VERSION 1.0.0 LANGUAGES CXX)

# Compiler configuration: Target strict C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# This tells CMake: If IWYU is installed (like on Ubuntu/macOS), use it.
# If not (like on Windows), skip it and don't crash.
find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(IWYU_PATH)
	set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
else()
	message(STATUS "IWYU not found - skipping include-what-you-use checks")
endif()

# ------------------------------------------------------------------------------
# 1. Core Engine Primitives (Shared across all games)
# ------------------------------------------------------------------------------
add_library(cards_core STATIC
    src/core/Card.cpp
    src/core/Deck.cpp
    src/core/Pile.cpp
)
target_include_directories(cards_core PUBLIC src)

# ------------------------------------------------------------------------------
# 2. Klondike Game Logic Library
# ------------------------------------------------------------------------------
add_library(klondike_game STATIC
    src/games/klondike/KlondikeBoard.cpp
    src/games/klondike/KlondikeRules.cpp
    src/games/klondike/KlondikeController.cpp
)
target_link_libraries(klondike_game PUBLIC cards_core)
target_include_directories(klondike_game PUBLIC src)

# Enforce strict flags on libraries
if(MSVC)
    target_compile_options(cards_core PRIVATE /W4 /WX /utf-8)
    target_compile_options(klondike_game PRIVATE /W4 /WX /utf-8)
else()
    target_compile_options(cards_core PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(klondike_game PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ------------------------------------------------------------------------------
# 3. CLI Executable (Links to the libraries)
# ------------------------------------------------------------------------------
add_executable(klondike_cli 
    src/cli/main.cpp 
    src/cli/ConsoleView.cpp
)
target_link_libraries(klondike_cli PRIVATE klondike_game)
target_include_directories(klondike_cli PRIVATE src) # Preprocessor include directories
target_compile_features(klondike_cli PRIVATE cxx_std_14) # Optional: Ensure it compiles with modern C++ standards

# Enforce strict code quality standards across GCC/Clang (Ubuntu/MacOS) and MSVC (Windows)
if(MSVC)
	target_compile_options(klondike_cli PRIVATE /W4 /WX /utf-8)
else()
	target_compile_options(klondike_cli PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Enable CTest and include the tests directory
enable_testing()
add_subdirectory(tests)

# ==========================================
# Documentation Target (Doxygen)
# ==========================================
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# Creates a custom 'doc' target that runs Doxygen using the existing Doxyfile
	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
	)
else()
	message(WARNING "Doxygen is not installed. Documentation cannot be generated.")
endif()