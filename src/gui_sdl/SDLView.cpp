/**
 * @file SDLView.cpp
 * @brief Implements the SDL2-based graphical rendering and event loop for the game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "SDLView.h"
#include <SDL2/SDL.h>        // for SDL_Quit, SDL_Init, SDL_INIT_VIDEO
#include <SDL2/SDL_error.h>  // for SDL_GetError
#include <SDL2/SDL_render.h> // for SDL_CreateRenderer, SDL_DestroyRenderer
#include <SDL2/SDL_video.h>  // for SDL_DestroyWindow, SDL_CreateWindow, SDL...
#include <SDL2/SDL_events.h> // for SDL_PollEvent, SDL_Event, SDL_EventType
#include <iostream>          // for basic_ostream, operator<<, char_traits
#include <stdexcept>         // for runtime_error

namespace core
{
    class IGameController;
}

namespace sdl
{

    SDLView::SDLView(core::IGameController &controller)
        : controller_(controller), window_(nullptr), renderer_(nullptr), isRunning_(false)
    {
        // Initialize the SDL2 video subsystem: SDL_INIT_VIDEO allows us to create windows and draw graphics.
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
            throw std::runtime_error("Failed to initialize SDL!");
        }

        // Create the SDL_Window: Center the window on the screen and set a resolution of 1024x768.
        window_ = SDL_CreateWindow(
            "Klondike Solitaire",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1024,
            768,
            SDL_WINDOW_SHOWN);

        if (!window_)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            SDL_Quit();
            throw std::runtime_error("Failed to create SDL Window!");
        }

        // Create the SDL_Renderer attached to the window for GPU drawing.
        // SDL_RENDERER_ACCELERATED tells SDL to use the hardware GPU for drawing.
        // SDL_RENDERER_PRESENTVSYNC syncs the framerate to the monitor's refresh rate (e.g., 60 FPS).
        renderer_ = SDL_CreateRenderer(
            window_,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!renderer_)
        {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(window_);
            SDL_Quit();
            throw std::runtime_error("Failed to create SDL Renderer!");
        }

        //  TODO: Load the sprite sheet texture containing the card images.
    }

    SDLView::~SDLView()
    {
        // Memory management in C requires strictly destroying resources
        // in the reverse order they were created.
        if (renderer_)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }

        if (window_)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }

        SDL_Quit(); // Cleanly shut down the entire SDL framework.

        // TODO: Destroy the sprite sheet texture.
    }

    void SDLView::run()
    {
        isRunning_ = true;

        // The core Game Loop: Process input, then draw the frame.
        while (isRunning_)
        {
            processEvents();
            render();

            // VSYNC is enabled in the renderer, so SDL_Delay is no longer strictly necessary to cap the framerate
        }
    }

    void SDLView::processEvents()
    {
        // struct that SDL fills with information about what the user just did (moved the mouse, pressed a key, minimized the window).
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            // Check what kind of event happened
            switch (event.type)
            {
            case SDL_QUIT:
                // This triggers when the user clicks the OS window's 'X' button
                isRunning_ = false;
                break;
                // TODO: add case SDL_MOUSEBUTTONDOWN: to click cards!

            default:
                break;
            }
        }
    }

    void SDLView::render()
    {
        // Set the renderer's paint color to a classic dark green
        // Arguments are (renderer, Red, Green, Blue, Alpha).
        SDL_SetRenderDrawColor(renderer_, 35, 120, 65, 255);

        // Clear the entire screen with the current paint color
        SDL_RenderClear(renderer_);

        // TODO: Iterate over the foundations, tableaus, stock, and waste using controller_.getPile().
        // TODO: Use SDL_RenderCopy() to draw specific card slices from the sprite sheet.

        // Swap the backbuffer to the front
        // SDL uses double-buffering to prevent screen tearing. All the drawing is done on a hidden background screen, and then 'Presented' to the user all at once.
        SDL_RenderPresent(renderer_);
    }

} // namespace sdl