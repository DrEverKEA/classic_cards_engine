/**
 * @file Pile.h
 * @brief Defines the Pile domain entity representing a physical stack of cards.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#pragma once

#include <vector>
#include "core/Card.h"

namespace core
{
    // A stricly ordered container for Card entities.
    // Game-rule agnostic: relies on external controllers to validate moves before appending.
    class Pile
    {
    public:
        // Explicitly telling the compiler to generate the standard default constructor
        // So when writting a custom constructor, the compiler won't take away the default one
        // Which in this case initializes the cards_ vector to be empty without writing it explicitly
        Pile() = default;

        // Single card mechanics (e.g. dealing a card to a tableau or foundation)
        /**
         * @brief Pushes a single card onto the top of the pile.
         * @param card Read-only reference to the Card being added.
         */
        void addCard(const Card &card);

        /**
         * @brief Pops and returns the top card from the pile.
         * @return The removed Card object.
         * @throws std::out_of_range if the pile is empty.
         */
        Card removeCard();

        // Multi-card mechanics (e.g. dragging a stack of 3 face-up cards to another tableau)
        /**
         * @brief Appends an ordered sub-stack of cards to the top of the pile.
         * @param newCards A vector representing the stack being moved.
         */
        void addCards(const std::vector<Card> &newCards);

        /**
         * @brief Slices off a specified number of cards from the top of the pile.
         * @param count The number of cards to remove.
         * @return A vector containing the removed cards, maintaining their original structural order.
         * @throws std::out_of_range if attempting to remove more cards than the pile contains.
         */
        std::vector<Card> removeCards(size_t count);

        // Read-only state accessors
        /**
         * @brief Retrieves a read-only reference to the top card without removing it.
         * @return Const reference to the top Card.
         * @throws std::out_of_range if the pile is empty.
         */
        const Card &getTopCard() const;

        /**
         * @brief Gets the total number of cards currently in the pile.
         * @return Integer representing the pile's size.
         */
        int size() const;

        /**
         * @brief Checks if the pile contains any cards.
         * @return True if empty, false otherwise.
         */
        bool isEmpty() const;

    private:
        std::vector<Card> cards_;
    };
}