/**
 * @file KlondikeBoard.cpp
 * @brief Implementation of the KlondikeBoard controller and standard deal logic.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/KlondikeBoard.h"
#include <stddef.h>    // for size_t
#include "core/Card.h" //for Card

namespace core
{

    KlondikeBoard::KlondikeBoard()
    {
        // The Deck constructor already created 52 ordered cards.
        // We must shuffle them to achieve true hardware-seeded entropy before dealing.
        stock_.suffle();
        initialDeal();
    }

    void KlondikeBoard::initialDeal()
    {
        // Outer loop: Iterate through all 7 tableaus
        for (size_t i = 0; i < tableaus_.size(); ++i)
        {
            // Inner loop: Deal 'i + 1' cards to the current tableau
            for (size_t j = 0; j <= i; ++j)
            {
                Card dealtCard = stock_.dealCard();
                // In standard Klondike, only the top (last) card placed on each tableau is face-up.
                if (j == i)
                {
                    dealtCard.flip();
                }
                tableaus_[i].addCard(dealtCard);
            }
        }
    }

    const Deck &KlondikeBoard::getStock() const
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
    Deck &KlondikeBoard::getStock()
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