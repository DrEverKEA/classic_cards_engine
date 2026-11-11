/**
 * @file KlondikeBoard.cpp
 * @brief Implementation of the KlondikeBoard controller and standard deal logic.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "games/klondike/KlondikeBoard.h"
#include <stddef.h>    // for size_t
#include "core/Card.h" //for Card

namespace core
{

    KlondikeBoard::KlondikeBoard()
    {
        initialDeal();
    }

    void KlondikeBoard::initialDeal()
    {
        // Create a fresh hardware-seeded deck locally and shuffle it
        Deck deck;
        deck.shuffle();

        // Outer loop: Iterate through all 7 tableaus
        for (size_t i = 0; i < tableaus_.size(); ++i)
        {
            // Inner loop: Deal 'i + 1' cards to the current tableau
            for (size_t j = 0; j <= i; ++j)
            {
                Card dealtCard = deck.dealCard();
                // Only the top (last) card placed on each tableau is face-up
                if (j == i)
                {
                    dealtCard.flip();
                }
                tableaus_[i].addCard(dealtCard);
            }
        }

        // Dump the remaining 24 cards into the stock pile
        while(!deck.isEmpty()){
            stock_.addCard(deck.dealCard());
        }

    }

    const Pile &KlondikeBoard::getStock() const
    {
        return stock_;
    }

    const Pile &KlondikeBoard::getWaste() const
    {
        return waste_;
    }

    const std::array<Pile, 4> &KlondikeBoard::getFoundations() const
    {
        return foundations_;
    }

    const std::array<Pile, 7> &KlondikeBoard::getTableaus() const
    {
        return tableaus_;
    }

    // Mutable accessors for the Controller to execute moves
    Pile &KlondikeBoard::getStock()
    {
        return stock_;
    }

    Pile &KlondikeBoard::getWaste()
    {
        return waste_;
    }

    std::array<Pile, 4> &KlondikeBoard::getFoundations()
    {
        return foundations_;
    }

    std::array<Pile, 7> &KlondikeBoard::getTableaus()
    {
        return tableaus_;
    }
} // namespace core