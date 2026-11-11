/**
 * @file IGameController.h
 * @brief Defines the abstract interface for all Solitaire game variants.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include "core/Pile.h"

namespace core
{
    /**
     * @enum PileGroup
     * @brief Categorizes piles across different Solitaire variations.
     *
     * Not all games use all groups. For example, FreeCell uses FreeCells but no Stock/Waste.
     * The View queries the controller to see which groups are active.
     */
    // strongly typed, scoped enumeration.
    enum class PileGroup
    {
        Tableau,
        Foundation,
        Stock,
        Waste,
        FreeCell
    };

    /**
     * @struct GameMove
     * @brief Standardized data packet for requesting a card move.
     */
    struct GameMove
    {
        PileGroup sourceGroup;
        int sourceIndex;
        PileGroup destGroup;
        int destIndex;
        int cardCount; // Default to 1, useful later for moving multiple cards (Spider/FreeCell)

        GameMove(PileGroup srcGrp, int srcIdx, PileGroup dstGrp, int dstIdx, int count = 1) : sourceGroup(srcGrp), sourceIndex(srcIdx), destGroup(dstGrp), destIndex(dstIdx), cardCount(count) {}
    };

    /**
     * @class IGameController
     * @brief The pure virtual base class defining the Game Engine API.
     */
    class IGameController
    {
    public:
        // Virtual destructor ensures proper cleanup of derived classes
        virtual ~IGameController() = default;

        // --------------------------------------------------------------------
        // Lifecycle Management
        // --------------------------------------------------------------------
        virtual void startGame() = 0;       // Shuffles and deals a new game.
        virtual void resetGame() = 0;       // Restarts the current game with the exact same initial shuffle.
        virtual bool isGameWon() const = 0; // Checks if the victory condition has been met.

        // --------------------------------------------------------------------
        // Action Routing
        // --------------------------------------------------------------------
        virtual bool attemptMove(const GameMove &move) = 0; // Attempts to execute a move requested by the View.
        virtual bool draw() = 0;                            // Specialized action for games with a drawable stock.

        // --------------------------------------------------------------------
        // State Interrogation (Read-Only)
        // --------------------------------------------------------------------
        virtual bool hasPileGroup(PileGroup group) const = 0;              // Checks if the specific game utilizes a particular pile group.
        virtual int getPileCount(PileGroup group) const = 0;               // Retrieves the number of piles within a specific group.
        virtual const Pile &getPile(PileGroup group, int index) const = 0; // Fetches a read-only reference to a specific pile for rendering.
    };
} // namespace core