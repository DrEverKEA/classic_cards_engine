/**
 * @file SDLView.h
 * @brief Defines the Graphical User Interface (GUI) view using SDL2 for the Klondike game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <SDL2/SDL_render.h> // for SDL_Renderer, SDL_Texture
#include <SDL2/SDL_video.h>  // for SDL_Window
#include <SDL2/SDL_rect.h>   // for SDL_Rect
namespace core
{
    class Card;
    class IGameController;
    enum class PileGroup;
    class Pile;
}

namespace sdl
{

    // Manages the hardware-accelerated 2D rendering window and mouse events.
    // This class acts as the View in the MVC architecture. It queries the
    // IGameController for the current game state and translates user mouse clicks
    // into GameMove objects to pass back to the controller.
    class SDLView
    {
    public:
        // Constructs the SDL GUI View.
        explicit SDLView(core::IGameController &controller);

        // Cleans up SDL window, renderer, and texture resources safely.
        ~SDLView();

        // Starts the main graphical game loop.
        // This loop continually processes hardware events (like mouse and keyboard input)
        // and triggers frame rendering until the application is closed.
        void run();

    private:
        core::IGameController &controller_; ///< The interface used to query state and attempt moves
        SDL_Window *window_;                ///< Pointer to the OS-level application window
        SDL_Renderer *renderer_;            ///< Pointer to the GPU hardware-accelerated renderer
        SDL_Texture *spriteSheet_;          ///< GPU texture holding the card images
        bool isRunning_;                    ///< Flag to keep the main game loop executing

        // Data packet for a resolved screen click
        struct ClickTarget
        {
            bool valid = false;
            core::PileGroup group;
            int pileIndex = 0;
            int cardIndex = -1;
        };

        // --- Selection State ---
        bool hasSelection_ = false;
        core::PileGroup selGroup_;
        int selPileIndex_ = 0;
        int selCardIndex_ = -1;

        // Helper methods

        // Polls the SDL event queue for user input.
        // Handles closing the window via the OS 'X' button and will eventually
        // handle drag-and-drop mouse events for card movement.
        void processEvents();

        // Clears the screen and draws the current game state.
        // Iterates through the board's piles via the controller and renders
        // the appropriate sprites from the texture atlas.
        void render();

        // Calculate the exact 64x64 slice on the sprite sheet for a given card
        SDL_Rect getCardSpriteRect(const core::Card &card) const;

        // Translates an (X, Y) screen coordinate into a game entity selection
        void handleMouseClick(int x, int y);
        void clearSelection();
        ClickTarget getClickTarget(int x, int y) const;
        void renderSelection();
        int getCardY(const core::Pile &tableau, int cardIndex) const;
    };
} // namespace sdl