/**
 * @file KlondikeBoard.h
 * @brief Defines the KlondikeBoard controller managing the standard Klondike layout and deal.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <array>
#include "core/Deck.h"
#include "core/Pile.h"

namespace core
{
    // The central state controller for a game of Klondike Solitaire.

    // Manages the physical layout of the game (stock, waste, foundations, tableaus)
    // and enforces the initial standard deal mechanics.
    class KlondikeBoard
    {
    public:
        // Constructs a new game board, shuffles the stock, and executes the standard deal.
        KlondikeBoard();

        // Read-only accessors for UI rendering and state validation
        const Pile &getStock() const;
        const Pile &getWaste() const;

        // std::array is used here because the exact number of these structures never fluctuates
        const std::array<Pile, 4> &getFoundations() const;
        const std::array<Pile, 7> &getTableaus() const;

        // Mutable accessors for the Controller to execute moves
        Pile &getStock();
        Pile &getWaste();
        std::array<Pile, 4> &getFoundations();
        std::array<Pile, 7> &getTableaus();

    private:
        Pile stock_;
        Pile waste_;
        std::array<Pile, 4> foundations_;
        std::array<Pile, 7> tableaus_;

        // Distributes 28 cards from the stock to the 7 tableaus (1 to the first, 2 to the second...).
        // Automatically flips the top card of each tableau face-up.
        void initialDeal();
    };
} // namespace core