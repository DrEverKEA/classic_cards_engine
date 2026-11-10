/**
 * @file main.cpp
 * @brief Main entry point for the Klondike Solitaire CLI application.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "games/klondike/KlondikeController.h"
#include "cli/ConsoleView.h"

int main()
{
    // 1. Initialize the Model/Controller (The Brain)
    core::KlondikeController controller;

    // 2. Initialize the View (The Face) and connect it to the Controller
    cli::ConsoleView view(controller);

    // 3. Start the game loop
    view.run();

    return 0;
}