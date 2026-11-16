/**
 * @file SDLView.cpp
 * @brief Implements the SDL2-based graphical rendering and event loop for the game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "SDLView.h"
#include <iostream>

namespace sdl
{

    SDLView::SDLView(core::IGameController &controller)
        : controller_(controller), window_(nullptr), renderer_(nullptr), isRunning_(false)
    {
        // TODO: Initialize the SDL2 video subsystem.
        // TODO: Create the SDL_Window (e.g., 800x600 resolution).
        // TODO: Create the SDL_Renderer attached to the window for GPU drawing.
        // TODO: Load the sprite sheet texture containing the card images.
    }

    SDLView::~SDLView()
    {
        // TODO: Destroy the sprite sheet texture.
        // TODO: Call SDL_DestroyRenderer(renderer_).
        // TODO: Call SDL_DestroyWindow(window_).
        // TODO: Call SDL_Quit() to cleanly shut down the SDL framework.
    }

    void SDLView::run()
    {
        isRunning_ = true;

        // The core Game Loop: Process input, then draw the frame.
        while (isRunning_)
        {
            processEvents();
            render();

            // TODO: Add SDL_Delay here later to cap the framerate (e.g., to 60 FPS)
            // so the game does not consume 100% of the CPU.
        }
    }

    void SDLView::processEvents()
    {
        // TODO: We will set up an SDL_Event object and use SDL_PollEvent(&event)
        // to catch things like SDL_QUIT (the user clicking the window's close button)
        // and SDL_MOUSEBUTTONDOWN (selecting a card).

        // Immediately exit for now so we don't get stuck in an infinite loop
        // while testing the initial compilation!
        isRunning_ = false;
    }

    void SDLView::render()
    {
        // TODO: Call SDL_RenderClear() to paint the background green.
        // TODO: Iterate over the foundations, tableaus, stock, and waste using controller_.getPile().
        // TODO: Use SDL_RenderCopy() to draw specific card slices from the sprite sheet.
        // TODO: Call SDL_RenderPresent() to swap the backbuffer and display the frame to the user.
    }

} // namespace sdl