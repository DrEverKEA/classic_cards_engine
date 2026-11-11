/**
 * @file KlondikeController.h
 * @brief Defines the Controller facade for the Klondike game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <cstddef>                        // for size_t
#include "games/klondike/KlondikeBoard.h" // for KlondikeBoard
#include "../../core/IGameController.h"

namespace core
{

    // The primary API for interacting with a game of Klondike Solitaire.
    // Acts as the Controller in the MVC architecture. It translates external
    // user intents into physical moves on the KlondikeBoard, validating all actions
    // against the KlondikeRules before execution.
    class KlondikeController : public core::IGameController
    {
    public:
        KlondikeController() = default;

        // --------------------------------------------------------------------
        // Lifecycle Management
        // --------------------------------------------------------------------
        void startGame() override;       // Shuffles and deals a new game.
        void resetGame() override;       // Restarts the current game with the exact same initial shuffle.
        bool isGameWon() const override; // Checks if the victory condition has been met.

        // --------------------------------------------------------------------
        // Action Routing
        // --------------------------------------------------------------------
        bool attemptMove(const GameMove &move) override; // Attempts to execute a move requested by the View.
        bool draw() override;                            // Specialized action for games with a drawable stock.

        // --------------------------------------------------------------------
        // State Interrogation (Read-Only)
        // --------------------------------------------------------------------
        bool hasPileGroup(PileGroup group) const override;              // Checks if the specific game utilizes a particular pile group.
        int getPileCount(PileGroup group) const override;               // Retrieves the number of piles within a specific group.
        const Pile &getPile(PileGroup group, int index) const override; // Fetches a read-only reference to a specific pile for rendering.

    private:
        // The controller owns and manipulates the physical board
        KlondikeBoard board_;
    };

} // namespace core