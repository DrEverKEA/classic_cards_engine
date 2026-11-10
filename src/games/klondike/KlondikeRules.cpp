/**
 * @file KlondikeRules.cpp
 * @brief Implementation of the stateless Klondike rules validator.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "games/klondike/KlondikeRules.h"
#include "core/Card.h" // for Card, Rank, Suit
#include "core/Pile.h" // for Pile

namespace core
{

    bool KlondikeRules::areAlternateColors(const Card &a, const Card &b)
    {
        // Helper lambda to determine if a suit is red
        auto isRed = [](Suit s)
        {
            return s == Suit::Diamonds || s == Suit::Hearts;
        };

        // If one is red and the other is not red (black), they alternate
        return isRed(a.getSuit()) != isRed(b.getSuit());
    }
    bool KlondikeRules::canMoveToTableau(const Card &cardToMove, const Pile &destination)
    {
        // Rule 1: Empty tableaus only accept Kings
        if (destination.isEmpty())
        {
            return cardToMove.getRank() == Rank::King;
        }

        const Card &topCard = destination.getTopCard();

        // Cannot attach to a face-down card
        if (!topCard.isFaceUp())
        {
            return false;
        }

        // Rule 2: Must be alternating color and exactly one rank lower
        bool alternateColor = areAlternateColors(cardToMove, topCard);

        // Using static_cast to safely do integer math on the strongly-typed enum
        bool descendingRank = static_cast<int>(topCard.getRank()) == (static_cast<int>(cardToMove.getRank()) + 1);

        return alternateColor && descendingRank;
    }
    bool KlondikeRules::canMoveToFoundation(const Card &cardToMove, const Pile &destination)
    {
        // Rule 1: Empty foundations only accept Aces
        if (destination.isEmpty())
        {
            return cardToMove.getRank() == Rank::Ace;
        }

        const Card &topCard = destination.getTopCard();

        // Rule 2: Must be the exact same suit and exactly one rank higher
        bool sameSuit = cardToMove.getSuit() == topCard.getSuit();
        bool ascendingRank = static_cast<int>(topCard.getRank()) == (static_cast<int>(cardToMove.getRank()) - 1);

        return sameSuit && ascendingRank;
    }

} // namespace core