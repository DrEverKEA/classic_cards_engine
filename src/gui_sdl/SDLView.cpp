/**
 * @file SDLView.cpp
 * @brief Implements the SDL2-based graphical rendering and event loop for the game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "SDLView.h"
#include <SDL2/SDL.h>             // for SDL_Quit, SDL_Init, SDL_INIT_VIDEO
#include <SDL2/SDL_error.h>       // for SDL_GetError
#include <SDL2/SDL_events.h>      // for SDL_PollEvent, SDL_Event, SDL_Even...
#include <SDL2/SDL_image.h>       // for IMG_Quit, IMG_GetError, IMG_Init
#include <SDL2/SDL_rect.h>        // for SDL_Rect
#include <SDL2/SDL_render.h>      // for SDL_RenderCopy, SDL_CreateRenderer
#include <SDL2/SDL_video.h>       // for SDL_DestroyWindow, SDL_CreateWindow
#include <iostream>               // for basic_ostream, operator<<, char_tr...
#include <stdexcept>              // for runtime_error
#include <vector>                 // for vector
#include "core/Card.h"            // for Card, Suit
#include "core/IGameController.h" // for IGameController, PileGroup
#include "core/Pile.h"            // for Pile

namespace
{
    // Sprite Sheet Configuration
    constexpr int SPRITE_SIZE = 64;
    constexpr int EMPTY_SLOT_COL = 13;
    constexpr int EMPTY_SLOT_ROW = 0;
    constexpr int CARD_BACK_COL = 13;
    constexpr int CARD_BACK_ROW = 1;

    // Render Layout Configuration
    constexpr int RENDER_WIDTH = 96;
    constexpr int RENDER_HEIGHT = 96;
    constexpr int START_X = 60;
    constexpr int START_Y = 50;
    constexpr int GAP_X = 130;
    constexpr int TABLEAU_Y = 180;
    constexpr int Y_OFFSET_FACEDOWN = 28;
    constexpr int Y_OFFSET_FACEUP = 38;
} // anonymous namespace

namespace sdl
{

    SDLView::SDLView(core::IGameController &controller)
        : controller_(controller), window_(nullptr), renderer_(nullptr), spriteSheet_(nullptr), isRunning_(false)
    {
        // Initialize the SDL2 video subsystem: SDL_INIT_VIDEO allows us to create windows and draw graphics.
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
            throw std::runtime_error("Failed to initialize SDL!");
        }

        // Initialize SDL_image for PNG loading
        int imgFlags = IMG_INIT_PNG;

        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << "\n";
            SDL_Quit();
            throw std::runtime_error("Failed to initialize SDL_image!");
        }

        // Create the SDL_Window: Center the window on the screen and set a resolution of 1024x768.
        window_ = SDL_CreateWindow(
            "Klondike Solitaire",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1024,
            768,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (!window_)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            IMG_Quit();
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
            IMG_Quit();
            SDL_Quit();
            throw std::runtime_error("Failed to create SDL Renderer!");
        }

        // Automatically scale the viewport if the window is resized
        SDL_RenderSetLogicalSize(renderer_, 1024, 768);

        // Load the sprite sheet texture containing the card images.
        // IMPORTANT: Assuming running the executable is from inside the /build folder,
        // it needs to step up one directory to reach /assets.
        spriteSheet_ = IMG_LoadTexture(renderer_, "../assets/cardsLarge_tilemap_packed.png");

        if (!spriteSheet_)
        {
            std::cerr << "Failed to load sprite sheet! IMG_Error: " << IMG_GetError() << "\n";
            // TODO: throw error
        }
    }

    SDLView::~SDLView()
    {
        if (spriteSheet_)
        {
            SDL_DestroyTexture(spriteSheet_);
            spriteSheet_ = nullptr;
        }

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

        IMG_Quit();
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

        // The blank card outline on the sprite sheet for empty piles (Row 0, Col 13)
        SDL_Rect emptySrc = { EMPTY_SLOT_COL * SPRITE_SIZE, EMPTY_SLOT_ROW * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };

        // --- Draw Stock ---
        SDL_Rect stockDest = { START_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT };
        const auto &stock = controller_.getPile(core::PileGroup::Stock, 0);

        if (stock.isEmpty())
        {
            SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &stockDest);
        }
        else
        {
            for (const auto &card : stock.getCards())
            {
                SDL_Rect src = getCardSpriteRect(card);
                SDL_RenderCopy(renderer_, spriteSheet_, &src, &stockDest);
            }
        }

        // --- Draw Waste ---
        SDL_Rect wasteDest = { START_X + GAP_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT };
        const auto &waste = controller_.getPile(core::PileGroup::Waste, 0);
        if (waste.isEmpty())
        {
            SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &wasteDest);
        }
        else
        {
            for (const auto &card : waste.getCards())
            {
                SDL_Rect src = getCardSpriteRect(card);
                SDL_RenderCopy(renderer_, spriteSheet_, &src, &wasteDest);
            }
        }

        // --- Draw Foundations ---
        for (int i = 0; i < 4; ++i)
        {
            SDL_Rect foundDst = {START_X + (3 + i) * GAP_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT};
            const auto &foundation = controller_.getPile(core::PileGroup::Foundation, i);

            if (foundation.isEmpty())
            {
                SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &foundDst);
            }
            else
            {
                for (const auto &card : foundation.getCards())
                {
                    SDL_Rect src = getCardSpriteRect(card);
                    SDL_RenderCopy(renderer_, spriteSheet_, &src, &foundDst);
                }
            }
        }

        // --- Draw Tableaus ---
        for (int i = 0; i < 7; ++i)
        {
            int currentX = START_X + i * GAP_X;
            int currentY = TABLEAU_Y; // Reset Y position for each column
            
            const auto &tableau = controller_.getPile(core::PileGroup::Tableau, i);

            if (tableau.isEmpty())
            {
                SDL_Rect tabDst = {currentX, currentY, RENDER_WIDTH, RENDER_HEIGHT};
                SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &tabDst);
            }
            else
            {
                for (const auto &card : tableau.getCards())
                {
                    SDL_Rect dst = {currentX, currentY, RENDER_WIDTH, RENDER_HEIGHT};
                    SDL_Rect src = getCardSpriteRect(card);
                    SDL_RenderCopy(renderer_, spriteSheet_, &src, &dst);

                    // Shift the Y position down for the next card in this pile
                    currentY += (card.isFaceUp() ? Y_OFFSET_FACEUP : Y_OFFSET_FACEDOWN);
                }
            }
        }

        // Swap the backbuffer to the front
        // SDL uses double-buffering to prevent screen tearing. All the drawing is done on a hidden background screen, and then 'Presented' to the user all at once.
        SDL_RenderPresent(renderer_);
    }

    SDL_Rect SDLView::getCardSpriteRect(const core::Card &card) const
    {
        // If the card is face down, always return the blue card back
        if (!card.isFaceUp())
        {
            return { CARD_BACK_COL * SPRITE_SIZE, CARD_BACK_ROW * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
        }

        // Map the Suit to the correct row on the sprite sheet
        int row = 0;
        switch (card.getSuit())
        {
        case core::Suit::Hearts:
            row = 0;
            break;
        case core::Suit::Diamonds:
            row = 1;
            break;
        case core::Suit::Clubs:
            row = 2;
            break;
        case core::Suit::Spades:
            row = 3;
            break;
        }

        // 3. Map the Rank to the correct column (0 to 12)
        int col = static_cast<int>(card.getRank()) - 1;

        // Return the calculated 64x64 source rectangle
        return { col * SPRITE_SIZE, row * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE };
    }

} // namespace sdl