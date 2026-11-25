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
#include <SDL2/SDL_events.h>      // for SDL_EventType, SDL_PollEvent, SDL_...
#include <SDL2/SDL_image.h>       // for IMG_Quit, IMG_GetError, IMG_Init
#include <SDL2/SDL_rect.h>        // for SDL_Rect
#include <SDL2/SDL_render.h>      // for SDL_RenderCopy, SDL_CreateRenderer
#include <SDL2/SDL_video.h>       // for SDL_DestroyWindow, SDL_CreateWindow
#include <SDL2/SDL_mouse.h>       // for SDL_BUTTON_LEFT
#include <SDL2/SDL_blendmode.h>   // for SDL_BlendMode
#include <SDL2/SDL_keycode.h>     // for SDL_KeyCode
#include <stddef.h>               // for size_t
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
        : controller_(controller), window_(nullptr), renderer_(nullptr), spriteSheet_(nullptr), isRunning_(false), dragGroup_(core::PileGroup::Tableau)
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

            case SDL_KEYDOWN:
                // Catch the 'R' key for Reset
                if (event.key.keysym.sym == SDLK_r)
                {
                    controller_.resetGame();
                    clearDrag();
                    std::cout << "Game Reset!\n";
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                // Only register Left Clicks
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    // event.button.x and .y are already perfectly scaled to 1024x768!
                    handleMouseDown(event.button.x, event.button.y);
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    handleMouseUp(event.button.x, event.button.y);
                }
                break;

            case SDL_MOUSEMOTION:
                handleMouseMotion(event.motion.x, event.motion.y);
                break;

            default:
                break;
            }
        }
    }

    void SDLView::render()
    {
        // Clear the entire absolute window with BLACK (creates clean dead zones)
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

        // Clear the entire screen with the current paint color
        SDL_RenderClear(renderer_);

        // Draw the Green Felt only over the 1024x768 logical game area
        SDL_SetRenderDrawColor(renderer_, 35, 120, 65, 255);
        SDL_Rect feltRect = {0, 0, 1024, 768};
        SDL_RenderFillRect(renderer_, &feltRect);

        // The blank card outline on the sprite sheet for empty piles (Row 0, Col 13)
        SDL_Rect emptySrc = {EMPTY_SLOT_COL * SPRITE_SIZE, EMPTY_SLOT_ROW * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE};

        // --- Draw Stock ---
        SDL_Rect stockDest = {START_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT};
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
        SDL_Rect wasteDest = {START_X + GAP_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT};
        const auto &waste = controller_.getPile(core::PileGroup::Waste, 0);
        const auto &wasteCards = waste.getCards();
        size_t wasteDrawCount = wasteCards.size();

        if (isDragging_ && dragGroup_ == core::PileGroup::Waste && wasteDrawCount > 0)
        {
            wasteDrawCount--; // Hide top card while dragging
        }

        if (wasteDrawCount == 0)
        {
            SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &wasteDest);
        }
        else
        {
            SDL_Rect src = getCardSpriteRect(wasteCards[wasteDrawCount - 1]);
            SDL_RenderCopy(renderer_, spriteSheet_, &src, &wasteDest);
        }

        // --- Draw Foundations ---
        for (int i = 0; i < 4; ++i)
        {
            SDL_Rect foundationDest = {START_X + (3 + i) * GAP_X, START_Y, RENDER_WIDTH, RENDER_HEIGHT};
            const auto &foundation = controller_.getPile(core::PileGroup::Foundation, i);
            const auto &foundationCards = foundation.getCards();
            size_t foundationDrawCount = foundationCards.size();

            if (isDragging_ && dragGroup_ == core::PileGroup::Foundation && dragPileIndex_ == i && foundationDrawCount > 0)
            {
                foundationDrawCount--; // Hide top card while dragging
            }

            if (foundationDrawCount == 0)
            {
                SDL_RenderCopy(renderer_, spriteSheet_, &emptySrc, &foundationDest);
            }
            else
            {
                SDL_Rect src = getCardSpriteRect(foundationCards[foundationDrawCount - 1]);
                SDL_RenderCopy(renderer_, spriteSheet_, &src, &foundationDest);
            }
        }

        // --- Draw Tableaus ---
        for (int i = 0; i < 7; ++i)
        {
            int tabX = START_X + i * GAP_X;
            const auto &tableau = controller_.getPile(core::PileGroup::Tableau, i);
            const auto &cards = tableau.getCards();

            for (size_t c = 0; c < cards.size(); ++c)
            {
                // Skip cards currently being dragged
                if (isDragging_ && dragGroup_ == core::PileGroup::Tableau &&
                    dragPileIndex_ == i && c >= static_cast<size_t>(dragCardIndex_))
                {
                    continue;
                }

                int cardY = getCardY(tableau, static_cast<int>(c));

                SDL_Rect destRect = {tabX, cardY, RENDER_WIDTH, RENDER_HEIGHT};
                SDL_Rect srcRect = getCardSpriteRect(cards[c]);
                SDL_RenderCopy(renderer_, spriteSheet_, &srcRect, &destRect);
            }
        }

        // Draw the visual selection highlight on top of everything
        renderDraggedCards();

        // --- WIN SCREEN OVERLAY & TEXT ---
        if (controller_.isGameWon())
        {
            SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer_, 255, 215, 0, 150);                     // Gold, 60% opacity
            SDL_RenderFillRect(renderer_, nullptr);                                  // Passing nullptr automatically fills the entire 1024x768 logical area
            SDL_SetWindowTitle(window_, "YOU WON, MATE! - Hit R to reset the Game"); // Instant UI Text Hack: Update the OS Window Title
        }
        else
        {
            SDL_SetWindowTitle(window_, "Klondike Solitaire"); // Keep the standard title during normal play
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
            return {CARD_BACK_COL * SPRITE_SIZE, CARD_BACK_ROW * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE};
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
        return {col * SPRITE_SIZE, row * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE};
    }

    SDLView::ClickTarget SDLView::getClickTarget(int x, int y) const
    {
        ClickTarget target;
        // Check Stock
        if (x >= START_X && x <= START_X + RENDER_WIDTH &&
            y >= START_Y && y <= START_Y + RENDER_HEIGHT)
        {
            target.valid = true;
            target.group = core::PileGroup::Stock;
            return target;
        }

        // Check Waste
        if (x >= START_X + GAP_X && x <= START_X + GAP_X + RENDER_WIDTH &&
            y >= START_Y && y <= START_Y + RENDER_HEIGHT)
        {
            target.valid = true;
            target.group = core::PileGroup::Waste;
            return target;
        }

        // Check Foundations
        for (int i = 0; i < 4; ++i)
        {
            int foundX = START_X + (3 + i) * GAP_X;
            // Add a generous +50 pixel padding to the bottom/sides of the hitbox
            if (x >= foundX - 10 && x <= foundX + RENDER_WIDTH + 10 &&
                y >= START_Y - 10 && y <= START_Y + RENDER_HEIGHT + 50)
            {
                target.valid = true;
                target.group = core::PileGroup::Foundation;
                target.pileIndex = i;
                return target;
            }
        }

        // Check Tableaus
        for (int i = 0; i < 7; ++i)
        {
            int tabX = START_X + i * GAP_X;
            if (x >= tabX && x <= tabX + RENDER_WIDTH)
            {
                const auto &tableau = controller_.getPile(core::PileGroup::Tableau, i);
                int clickedCardIdx = -1;

                if (!tableau.isEmpty())
                {
                    const auto &cards = tableau.getCards();
                    // Loop backwards (top-most visual card first)
                    for (int c = static_cast<int>(cards.size()) - 1; c >= 0; --c)
                    {
                        int cardY = getCardY(tableau, c);
                        if (y >= cardY && y <= cardY + RENDER_HEIGHT)
                        {
                            clickedCardIdx = c;
                            break; // Instantly lock in the hit
                        }
                    }
                }

                // UX FIX: If they clicked the column but missed the exact card rects, snap to bottom
                if (clickedCardIdx == -1 && y >= TABLEAU_Y)
                {
                    clickedCardIdx = tableau.isEmpty() ? -1 : static_cast<int>(tableau.getCards().size() - 1);
                }

                if (clickedCardIdx != -1 || tableau.isEmpty())
                {
                    target.valid = true;
                    target.group = core::PileGroup::Tableau;
                    target.pileIndex = i;
                    target.cardIndex = clickedCardIdx;
                    return target;
                }
            }
        }

        return target; // Returns invalid if clicked empty green felt
    }

    void SDLView::clearDrag()
    {
        isDragging_ = false;
        dragPileIndex_ = 0;
        dragCardIndex_ = -1;
    }

    void SDLView::handleMouseMotion(int x, int y)
    {
        if (isDragging_)
        {
            mouseX_ = x;
            mouseY_ = y;
        }
    }

    void SDLView::handleMouseDown(int x, int y)
    {

        // Freeze inputs if the engine says we won!
        if (controller_.isGameWon())
            return;

        ClickTarget target = getClickTarget(x, y);

        // If they clicked the green felt, cancel any active selection
        if (!target.valid)
        {
            return;
        }

        // --- IMMEDIATE ACTION: DRAW FROM STOCK ---
        // The Stock pile doesn't need a two-click destination. Clicking it draws a card.
        if (target.group == core::PileGroup::Stock)
        {
            controller_.draw();
            return;
        }

        // --- Validate source pile ---
        const auto &targetPile = controller_.getPile(target.group, target.pileIndex);
        if (targetPile.isEmpty())
            return;

        // Prevent dragging face-down cards in the Tableau
        if (target.cardIndex != -1 && !targetPile.getCards()[target.cardIndex].isFaceUp())
            return;

        // Lock in the selection
        isDragging_ = true;
        dragGroup_ = target.group;
        dragPileIndex_ = target.pileIndex;
        dragCardIndex_ = target.cardIndex;
        mouseX_ = x;
        mouseY_ = y;

        // Calculate grab offset so the card stays pinned where the cursor clicked it
        int cardBaseX = START_X;
        int cardBaseY = START_Y;

        if (dragGroup_ == core::PileGroup::Tableau)
        {
            cardBaseX = START_X + dragPileIndex_ * GAP_X;
            cardBaseY = getCardY(targetPile, dragCardIndex_);
        }
        else if (dragGroup_ == core::PileGroup::Waste)
        {
            cardBaseX = START_X + GAP_X;
        }
        else if (dragGroup_ == core::PileGroup::Foundation)
        {
            cardBaseX = START_X + (3 + dragPileIndex_) * GAP_X;
        }

        dragOffsetX_ = x - cardBaseX;
        dragOffsetY_ = y - cardBaseY;
    }

    void SDLView::handleMouseUp(int x, int y)
    {
        if (!isDragging_)
            return;

        ClickTarget target = getClickTarget(x, y);

        // Attempt move if dropped on a valid destination different from the source
        if (target.valid && !(dragGroup_ == target.group && dragPileIndex_ == target.pileIndex))
        {
            int cardCount = 1;
            if (dragGroup_ == core::PileGroup::Tableau && dragCardIndex_ != -1)
            {
                const auto &srcPile = controller_.getPile(dragGroup_, dragPileIndex_);
                cardCount = static_cast<int>(srcPile.getCards().size()) - dragCardIndex_;
            }

            core::GameMove move(dragGroup_, dragPileIndex_, target.group, target.pileIndex, cardCount);
            controller_.attemptMove(move);
        }

        // Always clear drag (invalid moves automatically snap back on next frame)
        clearDrag();
    }

    void SDLView::renderDraggedCards()
    {
        if (!isDragging_)
            return;

        const auto &pile = controller_.getPile(dragGroup_, dragPileIndex_);
        const auto &cards = pile.getCards();

        if (cards.empty())
            return;

        int startIndex = dragCardIndex_;

        if (dragGroup_ != core::PileGroup::Tableau)
        {
            startIndex = static_cast<int>(cards.size()) - 1;
        }

        int drawX = mouseX_ - dragOffsetX_;
        int drawY = mouseY_ - dragOffsetY_;

        for (size_t c = static_cast<size_t>(startIndex); c < cards.size(); c++)
        {
            SDL_Rect destRect = {drawX, drawY, RENDER_WIDTH, RENDER_HEIGHT};
            SDL_Rect srcRect = getCardSpriteRect(cards[c]);
            SDL_RenderCopy(renderer_, spriteSheet_, &srcRect, &destRect);

            drawY += Y_OFFSET_FACEUP;
        }
    }

    int SDLView::getCardY(const core::Pile &tableau, int cardIndex) const
    {
        int y = TABLEAU_Y;
        const auto &cards = tableau.getCards();
        // Calculate the Y offset based on the cards *above* the requested index
        for (int i = 0; i < cardIndex; ++i)
        {
            y += (cards[i].isFaceUp() ? Y_OFFSET_FACEUP : Y_OFFSET_FACEDOWN);
        }
        return y;
    }
} // namespace sdl