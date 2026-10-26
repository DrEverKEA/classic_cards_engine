/**
 * @file Deck.h
 * @brief Defines the Deck class serving as a factory and stateful container for Cards.
 * 
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <vector>
#include "core/Card.h"

namespace core
{
    // Represents a standard 52-card deck. Acts as both a factory for initial Card generation
    // and a stateful container managing the supply of undealt cards.
    class Deck
    {
    public:
        // Initializes the deck with 52 ordered cards (cartesian product of all 4 Suits and 13 Ranks).
        Deck();

        // Randomizes the order of the remaining cards in the container using
        // a standard uniform random bit generator (URBG)
        void suffle();

        // Pops and returns the top card from the deck.
        // Throws std::out_of_range if attempting to deal from an empty deck to prevent Undefined Behavior (UB).
        Card dealCard();

        // Read-only accessors for deck state validation during deal mechanics.
        int remainingCards() const;
        bool isEmpty() const;

    private:
        // std::vector provides contiguous memory allocation, allowing highly cache-efficient
        // random access required by std::suffle, while easily shrinking via pop_back() during deals.
        std::vector<Card> cards_;
    };
} // namespace core