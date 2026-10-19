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
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Application source tree definition
set(SOURCES
    src/main.cpp
    src/core/Card.cpp
    src/core/Deck.cpp
    src/rules/Klondike.cpp
    src/ui/TextConsole.cpp
)

# Executable target generation
add_executable(${PROJECT_NAME} ${SOURCES})

# Preprocessor include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Enable CTest and include the tests directory
enable_testing()
add_subdirectory(tests)
