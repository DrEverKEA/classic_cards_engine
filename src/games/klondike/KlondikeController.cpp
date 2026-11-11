/**
 * @file KlondikeController.cpp
 * @brief Implementation of the MVC Controller and move execution logic.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "games/klondike/KlondikeController.h"
#include <array>                          // for array
#include <vector>                         // for vector
#include "core/Card.h"                    // for Card
#include "core/Deck.h"                    // for Deck
#include "games/klondike/KlondikeRules.h" // for KlondikeRules
#include "core/Pile.h"                    // for Pile
#include <stdexcept>

namespace core
{
    void KlondikeController::startGame()
    {
        board_ = KlondikeBoard(); // Trashes the old board and builds a fresh one
    }

    void KlondikeController::resetGame()
    {
        board_ = KlondikeBoard();
    }

    bool KlondikeController::isGameWon() const
    {
        for (int i = 0; i < 4; ++i)
        {
            if (board_.getFoundations()[i].size() != 13)
            {
                return false;
            }
        }
        return true;
    }

    bool KlondikeController::draw()
    {
        Pile &stock = board_.getStock();
        Pile &waste = board_.getWaste();

        if (!stock.isEmpty())
        {
            Card drawn = stock.removeCard();
            drawn.flip(); // Face up in the waste pile
            waste.addCard(drawn);
            return true;
        }
        else if (!waste.isEmpty())
        {

            while (!waste.isEmpty())
            {
                // Recycle waste back into stock
                Card recycled = waste.removeCard();
                recycled.flip(); // Face down in the stock
                stock.addCard(recycled);
            }
            return true;
        }
        return false; // Both are empty
    }

    bool KlondikeController::hasPileGroup(PileGroup group) const
    {
        return group == PileGroup::Tableau ||
               group == PileGroup::Foundation ||
               group == PileGroup::Stock ||
               group == PileGroup::Waste;
    }

    int KlondikeController::getPileCount(PileGroup group) const
    {
        switch (group)
        {
        case PileGroup::Tableau:
            return 7;
        case PileGroup::Foundation:
            return 4;
        case PileGroup::Stock:
            return 1;
        case PileGroup::Waste:
            return 1;
        default:
            return 0;
        }
    }

    const Pile &KlondikeController::getPile(PileGroup group, int index) const
    {
        switch (group)
        {
        case PileGroup::Tableau:
            return board_.getTableaus()[index];
        case PileGroup::Foundation:
            return board_.getFoundations()[index];
        case PileGroup::Stock:
            return board_.getStock();
        case PileGroup::Waste:
            return board_.getWaste();
        default:
            throw std::out_of_range("Invalid PileGroup requested!");
        }
    }

    bool KlondikeController::attemptMove(const GameMove &move)
    {
        // Waste -> Tableau
        if (move.sourceGroup == PileGroup::Waste && move.destGroup == PileGroup::Tableau)
        {
            if (board_.getWaste().isEmpty())
                return false;
            if (move.destIndex < 0 || move.destIndex >= 7)
                return false;

            const Card &topCard = board_.getWaste().getTopCard();
            if (KlondikeRules::canMoveToTableau(topCard, board_.getTableaus()[move.destIndex]))
            {
                board_.getTableaus()[move.destIndex].addCard(board_.getWaste().removeCard());
                return true;
            }
        }

        // Waste -> Foundation
        if (move.sourceGroup == PileGroup::Waste && move.destGroup == PileGroup::Foundation)
        {
            if (board_.getWaste().isEmpty())
                return false;
            if (move.destIndex < 0 || move.destIndex >= 4)
                return false;

            const Card &topCard = board_.getWaste().getTopCard();
            if (KlondikeRules::canMoveToFoundation(topCard, board_.getFoundations()[move.destIndex]))
            {
                board_.getFoundations()[move.destIndex].addCard(board_.getWaste().removeCard());
                return true;
            }
        }

        // Tableau -> Foundation
        if (move.sourceGroup == PileGroup::Tableau && move.destGroup == PileGroup::Foundation)
        {
            if (move.sourceIndex < 0 || move.sourceIndex >= 7)
                return false;
            if (move.destIndex < 0 || move.destIndex >= 4)
                return false;

            Pile &sourcePile = board_.getTableaus()[move.sourceIndex];
            if (sourcePile.isEmpty())
                return false;

            const Card &topCard = sourcePile.getTopCard();
            if (KlondikeRules::canMoveToFoundation(topCard, board_.getFoundations()[move.destIndex]))
            {
                board_.getFoundations()[move.destIndex].addCard(sourcePile.removeCard());
                // Auto-flip newly exposed card
                if (!sourcePile.isEmpty() && !sourcePile.getTopCard().isFaceUp())
                {
                    Card flipped = sourcePile.removeCard();
                    flipped.flip();
                    sourcePile.addCard(flipped);
                }
                return true;
            }
        }

        // Tableau -> Tableau (Multi-card)
        if (move.sourceGroup == PileGroup::Tableau && move.destGroup == PileGroup::Tableau)
        {
            if (move.sourceIndex < 0 || move.sourceIndex >= 7)
                return false;
            if (move.destIndex < 0 || move.destIndex >= 7)
                return false;
            if (move.sourceIndex == move.destIndex)
                return false;

            Pile &sourcePile = board_.getTableaus()[move.sourceIndex];
            Pile &destPile = board_.getTableaus()[move.destIndex];

            if (sourcePile.size() < move.cardCount)
                return false;

            // Reading is fine because getCards() returns a const reference
            const Card &baseMovingCard = sourcePile.getCards()[sourcePile.size() - move.cardCount];
            if (!baseMovingCard.isFaceUp())
                return false;

            if (KlondikeRules::canMoveToTableau(baseMovingCard, destPile))
            {
                std::vector<Card> movingCards = sourcePile.removeCards(move.cardCount);

                for (const Card &c : movingCards)
                {
                    destPile.addCard(c);
                }

                if (!sourcePile.isEmpty() && !sourcePile.getTopCard().isFaceUp())
                {
                    Card flipped = sourcePile.removeCard();
                    flipped.flip();
                    sourcePile.addCard(flipped);
                }

                return true;
            }
        }

        return false;
    }
} // namespace core