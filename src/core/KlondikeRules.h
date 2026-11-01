/**
 * @file KlondikeRules.h
 * @brief Defines the stateless rule validator for Klondike Solitaire mechanics.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include "core/Card.h"
#include "core/Pile.h"

namespace core
{

    /**
     * @class KlondikeRules
     * @brief A stateless utility class that evaluates the legality of Klondike moves.
     *
     * @details Enforces alternating colors, descending ranks for tableaus, and
     * ascending matching suits for foundations. Does not modify game state.
     */
    class KlondikeRules
    {
    public:
        // Delete the default constructor to prevent instantiation. This is purely a utility class.
        KlondikeRules() = delete;

        /**
         * @brief Evaluates if a card can be legally placed onto a Tableau pile.
         * @param cardToMove The read-only card attempting to be placed.
         * @param destination The read-only tableau pile receiving the card.
         * @return True if the move is legal (descending rank, alternating color, or King on empty).
         */
        static bool canMoveToTableau(const Card &cardToMove, const Pile &destination);

        /**
         * @brief Evaluates if a card can be legally placed onto a Foundation pile.
         * @param cardToMove The read-only card attempting to be placed.
         * @param destination The read-only foundation pile receiving the card.
         * @return True if the move is legal (ascending rank of the same suit, or Ace on empty).
         */
        static bool canMoveToFoundation(const Card &cardToMove, const Pile &destination);

        /**
         * @brief Determines if two cards are of opposite colors (Red vs Black).
         * @param a Read-only first card.
         * @param b Read-only second card.
         * @return True if one is red and the other is black.
         */
        static bool areAlternateColors(const Card &a, const Card &b);
    };

} // namespace core