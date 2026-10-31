/**
 * @file test_board.cpp
 * @brief Unit tests for the KlondikeBoard controller and deal mechanics.
 *
 * @copyright Copyright (c) 2026. All rights reserved.
 *            This project is released under the MIT License.
 */

#include <gtest/gtest.h>
#include "core/KlondikeBoard.h"

using namespace core;

TEST(KlondikeBoardTest, InitialDealDistributesCardsCorrectly)
{
    KlondikeBoard board;

    // 52 total cards - 28 dealt to tableaus = 24 remaining in stock
    EXPECT_EQ(board.getStock().remainingCards(), 24);

    // Waste pile starts empty
    EXPECT_TRUE(board.getWaste().isEmpty());

    // All 4 foundations must be completely empty
    // auto: Tells the compiler to automatically deduce the exact type based on what the method returns.
    const auto &foundations = board.getFoundations();
    for (const auto &foundation : foundations)
    {
        EXPECT_TRUE(foundation.isEmpty());
    }

    // Tableaus must have an increasing number of cards (1 through 7)
    // and the top card of every tableau must be flipped face-up
    // auto: Tells the compiler to automatically deduce the exact type based on what the method returns.
    const auto &tableaus = board.getTableaus();
    for (size_t i = 0; i < tableaus.size(); i++)
    {
        // calling size() on a standard container, it returns a size_t, not a standard int
        // size_t is a special unsigned integer type designed specifically to represent sizes and memory counts.
        EXPECT_EQ(tableaus[i].size(), i + 1);
        EXPECT_TRUE(tableaus[i].getTopCard().isFaceUp());
    }
}