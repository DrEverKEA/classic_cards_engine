/**
 * @file KlondikeController.cpp
 * @brief Implementation of the MVC Controller and move execution logic.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/KlondikeController.h"

namespace core
{
    bool KlondikeController::draw()
    {
        Deck &stock = board_.getStock();
        Pile &waste = board_.getWaste();

        if (!stock.isEmpty())
        {
            Card drawn = stock.dealCard();
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

    bool KlondikeController::moveWasteToTableau(size_t tableauIndex)
    {
        if (tableauIndex >= 7 || board_.getWaste().isEmpty())
            return false;

        Pile &waste = board_.getWaste();
        Pile &tableau = board_.getTableaus()[tableauIndex];
        const Card &cardToMove = waste.getTopCard();

        if (KlondikeRules::canMoveToTableau(cardToMove, tableau))
        {
            tableau.addCard(waste.removeCard());
            return true;
        }
        return false;
    }

    bool KlondikeController::moveWasteToFoundation(size_t foundationIndex)
    {
        if (foundationIndex >= 4 || board_.getWaste().isEmpty())
            return false;

        Pile &waste = board_.getWaste();
        Pile &foundation = board_.getFoundations()[foundationIndex];
        const Card &cardToMove = waste.getTopCard();

        if (KlondikeRules::canMoveToFoundation(cardToMove, foundation))
        {
            foundation.addCard(waste.removeCard());
            return true;
        }
        return false;
    }

    bool KlondikeController::moveTableauToFoundation(size_t tableauIndex, size_t foundationIndex)
    {
        if (tableauIndex >= 7 || foundationIndex >= 4)
            return false;

        Pile &tableau = board_.getTableaus()[tableauIndex];
        if (tableau.isEmpty())
            return false;

        Pile &foundation = board_.getFoundations()[foundationIndex];
        const Card &cardToMove = tableau.getTopCard();

        if (KlondikeRules::canMoveToFoundation(cardToMove, foundation))
        {
            foundation.addCard(tableau.removeCard());

            // Auto-flip the newly exposed top card of the tableau
            if (!tableau.isEmpty() && !tableau.getTopCard().isFaceUp())
            {
                Card flipped = tableau.removeCard();
                flipped.flip();
                tableau.addCard(flipped);
            }
            return true;
        }
        return false;
    }

    bool KlondikeController::moveTableauToTableau(size_t sourceIndex, size_t destIndex, size_t count)
    {
        if (sourceIndex >= 7 || destIndex >= 7 || sourceIndex == destIndex || count == 0)
            return false;

        Pile &sourceTableau = board_.getTableaus()[sourceIndex];
        Pile &destTableau = board_.getTableaus()[destIndex];

        if (static_cast<size_t>(sourceTableau.size()) < count)
            return false;

        // Temporarily remove the stack to examine its bottom card safely
        std::vector<Card> movingStack = sourceTableau.removeCards(count);
        const Card &bottomOfStack = movingStack.front(); // The connection point

        if (KlondikeRules::canMoveToTableau(bottomOfStack, destTableau))
        {
            destTableau.addCards(movingStack);

            // Auto-flip the newly exposed top card of the source tableau
            if (!sourceTableau.isEmpty() && !sourceTableau.getTopCard().isFaceUp())
            {
                Card flipped = sourceTableau.removeCard();
                flipped.flip();
                sourceTableau.addCard(flipped);
            }
            return true;
        }
        else
        {
            // Move was illegal, revert the physical removal
            sourceTableau.addCards(movingStack);
            return false;
        }
    }

    const KlondikeBoard &KlondikeController::getBoard() const
    {
        return board_;
    }
} // namespace core