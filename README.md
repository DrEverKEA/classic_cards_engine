# Classic Cards Engine

A modern C++14 engine for classic solitaire games, built with a strict Model-View-Controller (MVC) architecture and Test-Driven Development (TDD). 

Currently, the engine features a fully playable Command Line Interface (CLI) version of Klondike Solitaire. The backend is completely decoupled from the view, allowing for future Graphical User Interface (GUI) extensions without altering the core game logic.

# Klondike Solitaire CLI

[![Klondike CLI Demo](https://asciinema.org/a/dl7DQsB8sJ1w8Zx6.svg)](https://asciinema.org/a/dl7DQsB8sJ1w8Zx6)

A fully-playable, terminal-based implementation of classic Klondike Solitaire, built with modern C++ and a strict MVC architecture.

## Features
* **Classic Rules:** Standard draw-1 mechanics, foundation building, and full tableau validation.
...

## Prerequisites

To build and run this project, you will need:
* A C++ compiler supporting **C++14** or higher
* **CMake** (version 3.20 or higher)
* A terminal that supports ANSI escape codes and Unicode (for colored card suits)

## Build Instructions

This project uses CMake for its build system. To compile the game, open your terminal and run the following commands from the project root:

```bash
# Create a dedicated build directory
mkdir build
cd build

# Configure the project
cmake ..

# Compile the executable
cmake --build .
```

## Running Tests

To verify the game logic and state management, run the automated test suite:
```bash
ctest --output-on-failure
```

## How to Play (CLI)

After a successful build, run the executable from inside your build directory:

./klondike_cli


### Game Commands

The CLI accepts text commands to move cards around the board. Enter a command and press Return.

    * d : Draw a card from the Stock to the Waste.

    * w t <T> : Move the top card from the Waste to Tableau <T> (e.g., w t 0).

    * w f <F> : Move the top card from the Waste to Foundation <F> (e.g., w f 0).

    * t f <T> <F> : Move the top card from Tableau <T> to Foundation <F> (e.g., t f 0 1).

    * t t <S> <D> <N> : Move <N> cards from the source Tableau <S> to the destination Tableau <D> (e.g., t t 0 1 3).

    * r : Restart with a new game.

    * q : Quit the game.

### Architecture Highlights

    Memory Safety: Completely avoids raw pointers (new/delete) and manual memory management by leveraging RAII and standard C++ containers (std::vector, std::array).

    MVC Pattern: The KlondikeController acts as a strict facade, keeping the ConsoleView completely ignorant of the underlying game mechanics (KlondikeRules, Deck, Pile).

    Strict Header Management: Integrated Include-What-You-Use (IWYU) directly into the CMake build pipeline to guarantee optimal dependency resolution and fast compilation times.