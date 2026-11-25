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

        // --- Drag and Drop State ---
        bool isDragging_ = false;
        core::PileGroup dragGroup_;
        int dragPileIndex_ = 0;
        int dragCardIndex_ = -1;

        // Mouse tracking & grab offsets (prevents top-left corner snapping)
        int mouseX_ = 0;
        int mouseY_ = 0;
        int dragOffsetX_ = 0;
        int dragOffsetY_ = 0;

        // Helper methods
        void processEvents();
        void render();

        void clearDrag();
        void handleMouseDown(int x, int y);
        void handleMouseUp(int x, int y);
        void handleMouseMotion(int x, int y);
        void renderDraggedCards();

        SDL_Rect getCardSpriteRect(const core::Card &card) const;
        ClickTarget getClickTarget(int x, int y) const;
        int getCardY(const core::Pile &tableau, int cardIndex) const;
    };
} // namespace sdl