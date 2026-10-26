/**
 * @file Deck.cpp
 * @brief Implementation of Deck generation, URBG shuffling, and LIFO deal mechanics.
 * 
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include "core/Deck.h"
#include <algorithm>
#include <random>
#include <stdexcept>

namespace core
{
    Deck::Deck()
    {
        // Pre-allocate memory to prevent dynamic reallocation overhead during population.
        // Because normally, when a vector runs out of space, it allocates a new, larger block of memory,
        // copies all existing items over, and deletes the old block.
        cards_.reserve(52);

        // Cartesian product generation mapping raw integers to domain enums,
        // since C++ does not allow direct iteration over enum class types out of the box.
        for (int suitInt = 0; suitInt < 4; ++suitInt)
        {
            for (int rankInt = 1; rankInt <= 13; ++rankInt)
            {
                // emplace_back constructs the Card directly in the vector's memory footprint,
                // avoiding temporary object instantiation and copy semantics.
                // with push_back() which expects an already-created object to copy into the vector
                cards_.emplace_back(
                    // This safely forces the raw integer (e.g., 1) back into our
                    // strongly-typed domain entity (Rank::Ace), (Suit::Clubs).
                    static_cast<Rank>(rankInt),
                    static_cast<Suit>(suitInt));
            }
        }
    }

    void Deck::suffle()
    {
        // Seed the Mersenne Twister engine with hardware entropy for uniform distribution.

        // Tap into the OS (or hardware) to generate true, unpredictable entropy (like measuring thermal noise or keystroke timing).
        std::random_device rd;
        // Use the true entropy to seed a Mersenne Twister (mt19937), which is the C++ industry standard for high-performance, statistically even pseudo-random number generation.
        std::mt19937 generator(rd());

        // This standard algorithm algorithmically swaps elements across the vector using the seeded generator created above.
        std::shuffle(cards_.begin(), cards_.end(), generator);
    }

    Card Deck::dealCard()
    {
        if (isEmpty())
        {
            throw std::out_of_range("Attempting to deal from an empty deck!");
        }

        // std::vector acts as a LIFO stack at its tail.
        // pop_back() returns void for exception safety (destroys the last element and shrinks the vector),
        // necessitating a prior copy via back(), which returns a reference to the last element in the array.
        Card dealt = cards_.back();
        cards_.pop_back();
        return dealt;
    }

    // Read-only accessors for deck state validation during deal mechanics.
    int Deck::remainingCards() const
    {
        // Cast size_t to int to align with domain logic and prevent signed/unsigned compiler warnings.
        return static_cast<int>(cards_.size());
    }
    bool Deck::isEmpty() const
    {
        // empty() is preferred over size() == 0 as it guarantees O(1) performance across all containers.
        // just checks if the begin() and end() pointers are at the same memory address.
        return cards_.empty();
    }
} // namespace core