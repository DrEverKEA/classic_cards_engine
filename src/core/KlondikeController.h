/**
 * @file KlondikeController.h
 * @brief Defines the Controller facade for the Klondike game.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <cstddef>               // for size_t
#include "core/KlondikeBoard.h"  // for KlondikeBoard

namespace core
{

    // The primary API for interacting with a game of Klondike Solitaire.
    // Acts as the Controller in the MVC architecture. It translates external
    // user intents into physical moves on the KlondikeBoard, validating all actions
    // against the KlondikeRules before execution.
    class KlondikeController
    {
    public:
        KlondikeController() = default;

        // Draws a card from the Stock to the Waste, or recycles the Waste to the Stock.
        // True if a draw or recycle occurred, false if both are empty.
        bool draw();

        // Attempts to move the top card from the Waste to a specific Tableau.
        // @param tableauIndex The index (0-6) of the destination tableau.
        // @return True if the move was legal and executed.
        bool moveWasteToTableau(size_t tableauIndex);

        // Attempts to move the top card from the Waste to a specific Foundation.
        // @param foundationIndex The index (0-3) of the destination foundation.
        // @return True if the move was legal and executed.
        bool moveWasteToFoundation(size_t foundationIndex);

        // Attempts to move the top card of a Tableau to a Foundation.
        // @param tableauIndex The index (0-6) of the source tableau.
        // @param foundationIndex The index (0-3) of the destination foundation.
        // @return True if the move was legal and executed.
        bool moveTableauToFoundation(size_t tableauIndex, size_t foundationIndex);

        // Attempts to move a face-up stack between Tableaus.
        // @param sourceIndex The index (0-6) of the source tableau.
        // @param destIndex The index (0-6) of the destination tableau.
        // @param count The number of face-up cards to move.
        // @return True if the move was legal and executed.
        bool moveTableauToTableau(size_t sourceIndex, size_t destIndex, size_t count);

        // Exposes the read-only state of the board so the View can render it.
        // @return Const reference to the KlondikeBoard.
        const KlondikeBoard &getBoard() const;

    private:
        // The controller owns and manipulates the physical board
        KlondikeBoard board_;
    };

} // namespace core