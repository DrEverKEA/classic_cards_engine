/**
 * @file main.cpp
 * @brief Entry point for the SDL2 graphical version of the Klondike game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "games/klondike/KlondikeController.h"
#include "SDLView.h"

int main(int argc, char* argv[])
{
    // 1. Initialize the Model/Controller (The Brain)
    core::KlondikeController controller;

    // 2. Initialize the View (The GUI) using the sdl namespace
    sdl::SDLView view(controller);

    // 3. Start the application lifecycle
    view.run();

    return 0;
}