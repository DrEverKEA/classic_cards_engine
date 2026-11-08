# ==============================================================================
# Project: classic_cards_engine
# Description: C++26 MVC Engine for Classic Solitaire Games
# 
# Reference Documentation for Build System:
# - Official CMake Docs: https://cmake.org/documentation/
# - Modern CMake Guide:  https://cliutils.gitlab.io/modern-cmake/
# ==============================================================================

cmake_minimum_required(VERSION 3.20)
project(classic_cards_engine VERSION 1.0.0 LANGUAGES CXX)

# Compiler configuration: Target strict C++26 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build the main CLI Game Executable
add_executable(klondike_cli 
    src/cli/main.cpp 
    src/cli/ConsoleView.cpp
    src/core/Card.cpp
    src/core/Deck.cpp
    src/core/Pile.cpp
    src/core/KlondikeBoard.cpp
    src/core/KlondikeRules.cpp
    src/core/KlondikeController.cpp
)

# Optional: Ensure it compiles with modern C++ standards
target_compile_features(klondike_cli PRIVATE cxx_std_14)

# Preprocessor include directories
target_include_directories(klondike_cli PRIVATE src)

# Enable CTest and include the tests directory
enable_testing()
add_subdirectory(tests)

# ==========================================
# Documentation Target (Doxygen)
# ==========================================
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Creates a custom 'doc' target that runs Doxygen using the existing Doxyfile
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else ()
    message(WARNING "Doxygen is not installed. Documentation cannot be generated.")
endif ()