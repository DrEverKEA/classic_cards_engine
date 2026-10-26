/**
 * @file Card.cpp
 * @brief Implementation of the Card domain entity's state and accessors.
 * 
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/Card.h"

namespace core
{
    // Utilizing a member initialization list to directly construct variables,
    // avoiding the performance overhead of default-construction followed by reassignment.
    Card::Card(Rank rank, Suit suit, bool isFaceUp)
        : rank_(rank), suit_(suit), isFaceUp_(isFaceUp)
    {
    }
    Rank Card::getRank() const
    {
        return rank_;
    }
    Suit Card::getSuit() const
    {
        return suit_;
    }
    bool Card::isFaceUp() const
    {
        return isFaceUp_;
    }

    // State mutator. Uses a domain-specific verb rather than a generic setter
    // to keep engine logic readable (e.g., card.flip()).
    void Card::flip()
    {
        isFaceUp_ = !isFaceUp_;
    }
} // namespace core