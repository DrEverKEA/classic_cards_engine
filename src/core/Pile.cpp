/**
 * @file Pile.cpp
 * @brief Implementation of the Pile entity's LIFO and sub-stack manipulation mechanics.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/Pile.h"
#include <stdexcept>

namespace core
{
    void Pile::addCard(const Card &card)
    {
        cards_.push_back(card);
    }

    Card Pile::removeCard()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Attempted to remove a card of an empty pile!");
        }

        Card top = cards_.back();
        cards_.pop_back();
        return top;
    }

    void Pile::addCards(const std::vector<Card> &newCards)
    {
        // std::vector::insert appends the entire external vector to the end of this vector
        // in a single, highly optimized memory operation.
        cards_.insert(cards_.end(), newCards.begin(), newCards.end());
    }

    std::vector<Card> Pile::removeCards(size_t count)
    {
        if (count > cards_.size())
        {
            throw std::out_of_range("Attempted to remove more cards than the pile contains.");
        }

        // Compute the iterator pointing to where the split begins
        // C++ compiler deduces the type at compile time (e.g. auto  is std::vector<Card>::iterator)
        auto splitIterator = cards_.end() - count;

        // Copy the requested sub-stack into a new vector, using the range constructor of std::vector
        // reading from the first iterator (inclusive) up to the second iterator (exclusive).
        std::vector<Card> removedStack(splitIterator, cards_.end());

        // Erase the sub-stack from the original container to prevent duplication
        cards_.erase(splitIterator, cards_.end());

        return removedStack;
    }

    const Card &Pile::getTopCard() const
    {
        if (isEmpty())
        {
            throw std::out_of_range("Attempted to get the top card of an empty pile!");
        }

        return cards_.back();
    }

    int Pile::size() const
    {
        return static_cast<int>(cards_.size());
    }

    bool Pile::isEmpty() const
    {
        return cards_.empty();
    }
}